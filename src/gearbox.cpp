#include "main.h"
#include "gearbox.hpp"
#include "lemlib/api.hpp"

// Define the motors that were declared as extern in the header
pros::Motor TopMotor(9, pros::E_MOTOR_GEARSET_36); // Using red cartridge (100 RPM) for better torque
pros::Motor BottomMotor(18, pros::E_MOTOR_GEARSET_36); // Using red cartridge (100 RPM) for better torque
pros::Rotation Arm_Sensor(8);

// PID Constants - adjusted for lemlib compatibility
const double kP = 0.5;  // Proportional gain (adjust as needed)
const double kI = 0.0;  // Integral gain (set to 0 for now)
const double kD = 0.1;  // Derivative gain (helps reduce overshoot)

// setup PID variables
static double error = 0, previousError = 0;
static double integral = 0, derivative = 0;
static double motorPower = 0;

const double Arm_Target = 1000; // adjust later for the proper loading angle
bool hold_arm = false; //keep track of if we hold the arm or not

void StopAll() { // stop all motion and disable the arm pid
    TopMotor.move(0);
    BottomMotor.move(0);
    hold_arm = false; 
    pros::delay(20); // proper motor sync
}

// make this function not interfere with the arm hold when loading rings
void Intake() { 
    if (hold_arm){ // move with 1 motor if the arm is being held
        BottomMotor.move(-127);
    }
    else {
        TopMotor.move(127);
        BottomMotor.move(-127); // move with 2 motors if the arm is not being held
    }
}

void MarryUp_and_FullOut() { // raise the arm and disable the arm pid
    TopMotor.move(-127);
    BottomMotor.move(127);
    hold_arm = false;
}

void Load_Arm_PID() {
    if (hold_arm) {
        double currentPosition = Arm_Sensor.get_position(); // Read sensor

        error = Arm_Target - currentPosition;
        integral += error;  // Accumulate error over time
        derivative = error - previousError; // Rate of error change

        motorPower = (kP * error) + (kI * integral) + (kD * derivative); 

        // Clamp motor power to valid range
        if (motorPower > 127) motorPower = 127;
        if (motorPower < -127) motorPower = -127;

        TopMotor.move(motorPower); // Move motor based on PID output

        previousError = error;
    }
    else {
        TopMotor.move(0);
        integral = 0;
    }
}

// stop holding position
void Kill_Arm() {
    hold_arm = false;
    integral = 0;
}

// main control loop to run in main
void Controll_Gears() {
    static pros::Controller master(pros::E_CONTROLLER_MASTER);
    static bool r1_prev = false;
    
    // Get current button states
    bool r1_curr = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool r2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    bool l2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    // R2: Intake
    if (r2) {
        Intake();
    }

    // R1: Toggle PID and load position (on button release)
    if (r1_curr && !r1_prev) {
        hold_arm = !hold_arm;
        if (!hold_arm) {
            Kill_Arm();
        }
    }
    if (hold_arm) {
        Load_Arm_PID();
    }
    r1_prev = r1_curr;

    // L2: Outtake and raise arm
    if (l2) {
        MarryUp_and_FullOut();
    }

    // If no buttons pressed and not holding arm position, stop motors
    if (!r2 && !l2 && !hold_arm) {
        StopAll();
    }
} 