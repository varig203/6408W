#include "main.h"
#include <string>  // For std::string
#include "gearbox.hpp"  // For function declarations
#include <cmath>   // For abs() and other math functions
#include "pros/adi.hpp"

// Add these function declarations at the top with other declarations
bool detect_red();
bool detect_blue();

pros::Motor TopMotor(9);
pros::Motor BottomMotor(18);
pros::Rotation Arm_Sensor(8);
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Optical Optical_Sensor(12);

const double Arm_Target = 310*100;  // 313 degrees * 100 (centidegrees)
const double acceptable_angle_range = 25;  // 1 degree * 100 (centidegrees)

const double kP = 0.05;  // Reduced from 5.0 to be less aggressive
const double kI = 0.0;  // Set to 0 until we get P working
const double kD = 0.1;  // Set to 0 until we get P working
double integral = 0;
double previousError = 0;

std::string current_state = "Nothing";

// Add at the top with other global variables
bool WillRedGetSorted = false;
bool up_button_prev = false;  // To track previous button state

void initialize_gearbox() {
    TopMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    BottomMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    
    // Initialize optical sensor
    Optical_Sensor.disable_gesture();  // We don't need gesture detection
    Optical_Sensor.set_led_pwm(100);   // Set LED brightness (0-100)
    
    // Reset and check rotation sensor
    Arm_Sensor.reset();
    pros::delay(100);
}

bool detect_red() {
    double hue = Optical_Sensor.get_hue();
    int proximity = Optical_Sensor.get_proximity();
    
    // Red hue is around 17 degrees, give it a small range (±5 degrees)
    bool is_red = (hue >= 12 && hue <= 22);
    
    return is_red;
}

bool detect_blue() {
    double hue = Optical_Sensor.get_hue();
    int proximity = Optical_Sensor.get_proximity();
    
    // Blue hue is around 217 degrees, give it a small range (±5 degrees)
    bool is_blue = (hue >= 212 && hue <= 222);
    
    return is_blue;
}
void stop_intake() {
    TopMotor.brake();
    BottomMotor.brake();
}

void auton_intake() {
    TopMotor.move(127);
    BottomMotor.move(-127);
}

void intake() {
    TopMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    BottomMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    

    // Get current sensor readings for debug
    double hue = Optical_Sensor.get_hue();
    bool red_detected = detect_red();
    bool blue_detected = detect_blue();

    if(WillRedGetSorted && red_detected) {
        current_state = "RED STOP";
        pros::delay(40);
        TopMotor.brake();
        BottomMotor.move(127);
        pros::delay(250);  // Hold for half a second
    }
    else if(!WillRedGetSorted && blue_detected) {
        current_state = "BLUE STOP";
        pros::delay(40);
        TopMotor.brake();
        BottomMotor.move(127);
        pros::delay(250);  // Hold for half a second
    }
    else {
        current_state = "INTAKING";
        TopMotor.move(127);
        BottomMotor.move(-127);
    }
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
    
    double motorPower = error * kP;
    motorPower = std::clamp(motorPower, -127.0, 127.0);
    TopMotor.move(motorPower);
    BottomMotor.move(-127);
    
    previousError = error;
}

void GearBox_Control() {
    // Check for up button press (edge detection)
    bool up_button_curr = master.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
    if (up_button_curr && !up_button_prev) {
        WillRedGetSorted = !WillRedGetSorted;  // Toggle the state
        master.print(0, 0, "Sorting: %s", WillRedGetSorted ? "RED" : "BLUE");  // Added controller print
    }
    up_button_prev = up_button_curr;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        current_state = "Intaking";
        intake();
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
        current_state = ".";
        TopMotor.brake();
        BottomMotor.brake();
    }
}

    

    
