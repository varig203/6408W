#include "main.h"

pros::Motor TopMotor(9);
pros::Motor BottomMotor(18);
pros::Rotation Arm_Sensor(8);

// PID Constants
const double kP = 0.5;  // Proportional gain (adjust as needed)
const double kI = 0.0;  // Integral gain (set to 0 for now)
const double kD = 0.1;  // Derivative gain (helps reduce overshoot)

static double error = 0, previousError = 0;
static double integral = 0, derivative = 0;
static double motorPower = 0;

const double Arm_Target = 1000; // tune the height of the arm to load rings into the marry brown 

bool hold_arm = false; //keep track of if we hold the arm or not


void StopAll() { // stop all motion and dissable the arm pid
    TopMotor.move(0);
    BottomMotor.move(0);
    hold_arm = false; 
}

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
}

void Load_Arm() {
    static double error = 0, previousError = 0;
    static double integral = 0, derivative = 0;
    static double motorPower = 0;
    while (hold_arm){
        pros::Task Load_ring(Load_Arm_PID);
    }
}

