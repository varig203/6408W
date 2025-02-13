#include "../include/gearbox.hpp"
#include "main.h"

pros::Motor TopMotor(9);
pros::Motor BottomMotor(18);
pros::Rotation Arm_Sensor(8);

// PID Constants
const double kP = 7;  // Proportional gain (adjust as needed)
const double kI = 0.4;  // Integral gain (set to 0 for now)
const double kD = 0.1;  // Derivative gain (helps reduce overshoot)

// setup PID variables
static double error = 0, previousError = 0;
static double integral = 0, derivative = 0;
static double motorPower = 0;

const double Arm_Target = 313; // target in degrees
const double TICKS_PER_DEG = 100.0; // VEX Rotation Sensor reports in centidegrees (100 per degree)
const double ACCEPTABLE_ANGLE_RANGE = 1.0; // Acceptable angle range for holding position (degrees)

bool hold_arm = false; //keep track of if we hold the arm or not

// Convert centidegrees to degrees
double get_degrees() {
    return Arm_Sensor.get_position() / TICKS_PER_DEG;
}

void StopAll() { // stop all motion and dissable the arm pid
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
        double currentPosition = get_degrees(); // Read sensor in degrees
        error = Arm_Target - currentPosition;
        
        // New check: if the arm is within the acceptable angle range of the target, then hold position.
        if (fabs(error) < ACCEPTABLE_ANGLE_RANGE) {
            TopMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);  // Set motor to hold mode
            TopMotor.move(0);                                   // Hold position
            integral = 0;           // Reset the integral to avoid windup
            previousError = error;  // Update previous error for consistency
            return;                 // Exit early without further PID calculation
        } else {
            TopMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); // Use coast mode for active PID control
        }
        
        integral += error;                      // Accumulate error over time
        derivative = error - previousError;     // Determine the rate-of-change
        
        motorPower = (kP * error) + (kI * integral) + (kD * derivative);
        
        TopMotor.move(motorPower);                // Command the motor based on PID output
        
        previousError = error;
        pros::delay(20); // Prevent CPU overload
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

// main controll loop to run in main
void Controll_Gears() {
    // Print rotation sensor value to controller in degrees
    pros::Controller(pros::E_CONTROLLER_MASTER).print(0, 0, "Deg: %.1f   ", get_degrees());

    if (pros::Controller(pros::E_CONTROLLER_MASTER).get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { // R2 intakes only while pressed
        Intake();
    } else {
        // Stop intake motors when R2 is released
        TopMotor.move(0);
        BottomMotor.move(0);
    }

    if (pros::Controller(pros::E_CONTROLLER_MASTER).get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // toggle the load position and unlock it with the same button

        if (hold_arm) {
            Kill_Arm();
        } else {
            hold_arm = true;
            Load_Arm_PID();
        }
    }

    if (pros::Controller(pros::E_CONTROLLER_MASTER).get_digital(pros::E_CONTROLLER_DIGITAL_L2)) { // L2 outtakes and raises the arm
        MarryUp_and_FullOut();
    }
} 