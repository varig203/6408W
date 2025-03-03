#include "main.h"
#include "gearbox.hpp"  // For function declarations

// Add these function declarations at the top with other declarations
bool isRedDetected();
bool isBlueDetected();

const double ARM_TARGET = 310*100;  // 313 degrees * 100 (centidegrees)
const double ACCEPTABLE_ANGLE_RANGE = 150;  // 1 degree * 100 (centidegrees)

const double kP = 0.05;  // Reduced from 5.0 to be less aggressive
const double kI = 0.0;  // Set to 0 until we get P working
const double kD = 0.1;  // Set to 0 until we get P working
double integral = 0;
double previousError = 0;

// Add at the top with other global variables
bool isRedSorted = false;
bool upButtonPrev = false;

void initializeGearbox_fn() {
    TopMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    BottomMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    
    // Initialize optical sensor
    OpticalSensor.disable_gesture();  // We don't need gesture detection
    OpticalSensor.set_led_pwm(100);

    Console.println("Whopper gaming (Motor temps)\n");
    Console.printf("TopMotor Temp: %lf\n", TopMotor.get_temperature());
    Console.printf("BottomMotor Temp: %lf\n", BottomMotor.get_temperature());
    
    // Reset and check rotation sensor
    ArmSensor.reset();
    pros::delay(100);
}

bool isRedDetected() {
    double hue = OpticalSensor.get_hue();
   
    // Red hue is around 17 degrees, give it a small range (±5 degrees)
    bool isRed = (hue >= 12 && hue <= 22);
    
    return isRed;
}

bool isBlueDetected() {
    double hue = OpticalSensor.get_hue();
    
    // Blue hue is around 217 degrees, give it a small range (±5 degrees)
    bool isBlue = (hue >= 212 && hue <= 222);
    
    return isBlue;
}
void stopIntake_fn() {
    TopMotor.brake();
    BottomMotor.brake();
}

void autonIntake_fn() {
    TopMotor.move(127);
    BottomMotor.move(-127);
}
void autonIntakeHandler_fn(int timeout_ms) {
    uint32_t timerStart = pros::millis();
    while ((pros::millis() - timerStart) < timeout_ms) {
        intake_fn();
        pros::delay(20);
    }
    stopIntake_fn();

}

void intake_fn() {
    TopMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    BottomMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    

    // Get current sensor readings for debug
    double hue = OpticalSensor.get_hue();
    bool redDetected = isRedDetected();
    bool blueDetected = isBlueDetected();

    if(isRedSorted && redDetected) {
        pros::delay(40);
        TopMotor.brake();
        BottomMotor.move(127);
        pros::delay(250);  // Hold for half a second
    } else if(!isRedSorted && blueDetected) {
        pros::delay(40);
        TopMotor.brake();
        BottomMotor.move(127);
        pros::delay(250);  // Hold for half a second
    } else {
        TopMotor.move(127);
        BottomMotor.move(-127);
    }
}

void raiseArm_fn() {
    TopMotor.move(-127);
    BottomMotor.move(127);
}

void setArmPosition_fn() {
    
    // Get sensor values
    int32_t raw_position = ArmSensor.get_position();
    double currentAngle = ArmSensor.get_position();  // This returns centidegrees
    double error = ARM_TARGET - currentAngle;
    
    if ((std::abs(error) <= ACCEPTABLE_ANGLE_RANGE) || raw_position < 20)   {  // Now checking if within 1 degree
        if (BottomMotor.get_actual_velocity() < 5||BottomMotor.get_actual_velocity() > -5){
            intake_fn();
        } else {
            TopMotor.brake();
            BottomMotor.move(-127);
            integral = 0;
        }

        return;
    }
    
    double motorPower = error * kP;
    motorPower = std::clamp(motorPower, -127.0, 127.0);
    TopMotor.move(motorPower);
    BottomMotor.move(-127);
    
    previousError = error;
}

void GearBox_Control() {
    // Check for up button press (edge detection)
    bool upButtonCurr = Controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
    if (upButtonCurr && !upButtonPrev) {
        isRedSorted = !isRedSorted;  // Toggle the state
        Controller.print(0, 0, "Sorting: %s", isRedSorted ? "RED" : "BLUE");  // Added controller print
    }
    upButtonPrev = upButtonCurr;

    if (Controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        intake_fn();

    } else if (Controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        setArmPosition_fn();
       
    } else if (Controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        raiseArm_fn();
        
    } else {
        TopMotor.brake();
        BottomMotor.brake();
        
    }
}