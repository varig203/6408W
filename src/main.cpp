#include "main.h"
#include "pnuematic.hpp"  // Include the declaration for clamp_fn and doinker
#include "../include/gearbox.hpp"    // Include the declaration for Controll_Gears    // Include the declaration for intake
#include "lemlib/api.hpp"  // Include lemlib API for chassis and drivetrain
#include "autons.hpp"
// Global drive objects under the lemlib framework:
pros::MotorGroup left_motor_group({5, -6, 7}, pros::MotorGearset::blue);
pros::MotorGroup right_motor_group({-1, 3, -4}, pros::MotorGearset::blue);

lemlib::Drivetrain drivetrain( &right_motor_group, &left_motor_group,11.5, lemlib::Omniwheel::NEW_325, 450, 2);
//https://lemlib.readthedocs.io/en/stable/tutorials/4_pid_tuning.html
//^ PID Tuning Guide
lemlib::ControllerSettings lateral_controller(20, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              170, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);
lemlib::ControllerSettings angular_controller(1.65, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              12, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);
pros::Rotation VerticalTracking(-16);
pros::Imu imu(19);
//lemlib::TrackingWheel horizontal_tracking_wheel(&VerticalTracking, lemlib::Omniwheel::NEW_1, -5.75);
lemlib::TrackingWheel vertical_tracking_wheel(&VerticalTracking, lemlib::Omniwheel::NEW_2, 1);
lemlib::OdomSensors sensors(&vertical_tracking_wheel, nullptr, nullptr, nullptr, &imu);
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     5 // expo curve gain
);
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);

// At the top after includes
ASSET(TestPath_txt);  // Declare the example.txt path file

// Setup function for drivetrain
void setup_drivetrain() {
	left_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	right_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	// Set up drivetrain brake modes
	setup_drivetrain();

	pros::lcd::register_btn1_cb(on_center_button);
	initialize_gearbox();
	chassis.calibrate();	

	// The global chassis object will be used in opcontrol()
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	// Optionally set the starting pose. In this example we keep it at the origin
	Blue_Goal_Auton();
}
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller controller(pros::E_CONTROLLER_MASTER);
	bool a_button_prev = false;
    bool b_button_prev = false;
	while (true) {
		bool a_button_curr = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		if (a_button_curr && !a_button_prev) {
			doinker();  // Toggle the G port by calling the doinker function
		}
		a_button_prev = a_button_curr;

		// Check if the B button was just pressed (edge detection)
		bool b_button_curr = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);
		if (b_button_curr && !b_button_prev) {
			clamp_fn();  // Toggle the H port by calling the clamp function
		}
		b_button_prev = b_button_curr;

		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		
		// Get throttle and turning values from the controller.
		// Invert the throttle value because pushing forward returns a negative value.
		int throttle = -controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		// Use the raw right stick value for turning.
		int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		// Use the global chassis object's arcade drive function.
		chassis.arcade(throttle, turn, false, 0.75);
		GearBox_Control();

		pros::delay(25);
	}
}