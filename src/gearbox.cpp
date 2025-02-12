#include "../include/gearbox.hpp"
#include "main.h"

pros::Motor TopMotor(9);
pros::Motor BottomMotor(18);
pros::Rotation Arm_Sensor(8);

// PID Constants
const double kP = 0.5;  // Proportional gain (adjust as needed)
const double kI = 0.0;  // Integral gain (set to 0 for now)
const double kD = 0.1;  // Derivative gain (helps reduce overshoot)

// setup PID variables
static double error = 0, previousError = 0;
static double integral = 0, derivative = 0;
static double motorPower = 0;

const double Arm_Target = 1000; // adjust later for the proper loading angle
bool hold_arm = false; //keep track of if we hold the arm or not

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
        double currentPosition = Arm_Sensor.get_position(); // Read sensor

        error = Arm_Target - currentPosition;
        integral += error;  // Accumulate error over time
        derivative = error - previousError; // Rate of error change

        motorPower = (kP * error) + (kI * integral) + (kD * derivative); 

        TopMotor.move(motorPower); // Move motor based on PID output

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
            Load_Arm_PID();
        }
    }

    if (pros::Controller(pros::E_CONTROLLER_MASTER).get_digital(pros::E_CONTROLLER_DIGITAL_L2)) { // L2 outtakes and raises the arm
        MarryUp_and_FullOut();
    }
} 