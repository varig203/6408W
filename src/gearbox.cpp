#include "main.h"
#include <string>  // For std::string
#include "gearbox.hpp"  // For function declarations
#include <cmath>   // For abs() and other math functions
#include "pros/adi.hpp"


pros::Motor TopMotor(9);
pros::Motor BottomMotor(18);
pros::Rotation Arm_Sensor(8);
pros::Controller master(pros::E_CONTROLLER_MASTER);

const double Arm_Target = 313;
const double acceptable_angle_range = 1;

const double kP = 0.5;  // Tune these values!
const double kI = 0.1;
const double kD = 0.2;
double integral = 0;
double previousError = 0;

std::string current_state = "Nothing";

void initialize_gearbox() {
    TopMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);  // Best for holding arm position
    BottomMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);  // Better for continuous spinning
}

void intake() {
    current_state = "Intaking";
    TopMotor.move(127);
    BottomMotor.move(-127);
}
void raise_arm() {
    current_state = "Arm Raising";
    TopMotor.move(-127);
    BottomMotor.move(127);
}


void set_arm_position() {
    double current_angle = Arm_Sensor.get_angle();
    double error = Arm_Target - current_angle;
    
    // Display debugging information on the V5 brain screen
    static uint32_t lastPrint = 0;
    if (pros::millis() - lastPrint > 50) {
        pros::lcd::print(0, "Target: %f", Arm_Target);
        pros::lcd::print(1, "Current: %f", current_angle);
        pros::lcd::print(2, "Error: %f", error);
        master.print(0, 0, "State: %s", current_state.c_str());  // Print to controller
        lastPrint = pros::millis();
    }
    
    if (abs(error) <= acceptable_angle_range) {
        current_state = "Arm Holding";
        TopMotor.brake();
        BottomMotor.move(127);
        integral = 0;
        return;
    }
    
    current_state = "Arm Raising";
    // Integral term: sums up past error to help overcome consistent forces (like gravity)
    // Limit how large this sum can get to prevent excessive correction
    const double maxIntegral = 50.0;
    integral += error;
    integral = std::clamp(integral, -maxIntegral, maxIntegral);  // Keep integral term within bounds
    
    // Derivative term: looks at how fast error is changing
    // Helps prevent overshooting by applying more correction when error changes quickly
    double derivative = error - previousError;
    
    // Calculate motor power using PID formula
    // P term: proportional to current error
    // I term: proportional to accumulated error
    // D term: proportional to rate of change of error
    double motorPower = (error * kP) + (integral * kI) + (derivative * kD);
    
    // Keep motor power within valid range (-127 to 127)
    motorPower = std::clamp(motorPower, -127.0, 127.0);
    
    // Apply power to motors
    // Negative for top motor because that's how it raises the arm
    TopMotor.move(-motorPower);
    BottomMotor.move(motorPower);
    
    // Store current error for next iteration's derivative calculation
    previousError = error;
}

void GearBox_Control() {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        //intaking 
        intake();
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        //setting arm pos
        set_arm_position();
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        //raising arm
        raise_arm();
    }
    else {
        current_state = "Nothing";
    }
    
    // Update controller display even when no buttons are pressed
    static uint32_t lastPrint = 0;
    if (pros::millis() - lastPrint > 50) {
        master.print(0, 0, "State: %s", current_state.c_str());
        lastPrint = pros::millis();
    }
}

    

    
