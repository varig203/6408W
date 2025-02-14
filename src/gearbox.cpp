#include "main.h"
#include <string>  // For std::string
#include "gearbox.hpp"  // For function declarations
#include <cmath>   // For abs() and other math functions
#include "pros/adi.hpp"


pros::Motor TopMotor(9);
pros::Motor BottomMotor(18);
pros::Rotation Arm_Sensor(8);
pros::Controller master(pros::E_CONTROLLER_MASTER);

const double Arm_Target = 310*100;  // 313 degrees * 100 (centidegrees)
const double acceptable_angle_range = 25;  // 1 degree * 100 (centidegrees)

const double kP = 0.05;  // Reduced from 5.0 to be less aggressive
const double kI = 0.0;  // Set to 0 until we get P working
const double kD = 0.1;  // Set to 0 until we get P working
double integral = 0;
double previousError = 0;

std::string current_state = "Nothing";

void initialize_gearbox() {
    TopMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    BottomMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    
    // Reset and check rotation sensor
    Arm_Sensor.reset();  // Reset the sensor to 0
    pros::delay(100);    // Give it time to reset
    
    // Print initial sensor status
    pros::lcd::print(0, "Sensor Init: %d", Arm_Sensor.is_installed());
}

void intake() {
    //current_state = "Intaking";
    TopMotor.move(127);
    BottomMotor.move(-127);
}
void raise_arm() {
    current_state = "Arm Raising";
    TopMotor.move(-127);
    BottomMotor.move(127);
}


void set_arm_position() {
    
    // Get sensor values
    int32_t raw_position = Arm_Sensor.get_position();
    double current_angle = Arm_Sensor.get_position();  // This returns centidegrees
    double error = Arm_Target - current_angle;
    
    // Debug printing
    
    
    if (abs(error) <= acceptable_angle_range) {  // Now checking if within 1 degree
        if(BottomMotor.get_actual_velocity() < 5||BottomMotor.get_actual_velocity() > -5){
            intake();
            current_state = "Arm moving velo 0 ";
        }
        else {
            current_state = "Arm Holding";
            TopMotor.brake();
            BottomMotor.move(-127);
            integral = 0;
        }

        return;
    }
    else {
        current_state = "Arm Moving";
    }
    
    //current_state = "Arm Moving";
    double motorPower = error * kP;
    motorPower = std::clamp(motorPower, -127.0, 127.0);
    TopMotor.move(motorPower);
    BottomMotor.move(-127);
    
    previousError = error;
}

void GearBox_Control() {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        current_state = "Intaking";
        TopMotor.move(127);
        BottomMotor.move(-127);
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        set_arm_position();
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        current_state = "Arm Raising";
        TopMotor.move(-127);
        BottomMotor.move(127);
    }
    else {
        // When no buttons are pressed, stop motors
        current_state = ".";
        TopMotor.brake();
        BottomMotor.brake();
    }
    static uint32_t lastPrint = 0;
    if (pros::millis() - lastPrint > 50) {
        
        pros::lcd::print(2, "Current Angle: %.2f", Arm_Sensor.get_position()/100);
        pros::lcd::print(3, "bottom motor velocity: %.2f", BottomMotor.get_actual_velocity());
        pros::lcd::print(4, "current state: %s", current_state.c_str());
        lastPrint = pros::millis();
    }
    // Update all displays (brain and controller)

}

    

    
