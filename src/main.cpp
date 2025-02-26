#include "main.h"
#include "pnuematic.hpp"  // Include the declaration for clamp_fn and doinker
#include "gearbox.hpp"    // Include the declaration for Controll_Gears    // Include the declaration for intake
#include "autons.hpp"
#include "team_logo.h"

/******************************
 *       DANIEL'S TODO:       *
 *       - CLEANUP CODE       *
 *     - CLEANUP COMMENTS     *
 * - RESTRUCTURE IF NECESSARY *
 *      - OPTIMIZE CODE       *
 ******************************/

// Auton selector setup
// Docs https://robodash.readthedocs.io/en/latest/
rd::Selector autonSelector({
	{"Blue Ring", Blue_Side_Auton},
	{"Blue Goal", Blue_Goal_Auton},
	{"Red Ring", Red_Side_Auton},
	{"Red Goals", Red_Goal_Auton},
	{"Skills", skills},
});

rd::Console console; // Creating the terminal
rd::Image image1(&team_logo,"Whopper"); // Creating the image

// At the top after includes
ASSET(TestPath_txt);  // Declare the example.txt path file

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Set up drivetrain brake modes
	left_motors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	right_motors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	// Initalize gearbox and calibrate drive sensors
	initialize_gearbox();
	chassis.calibrate();

	// Focus onto auton selector
	autonSelector.focus();
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
	skills(); // Shows picture for after auton
	//autonSelector.run_auton(); // REMOVE COMMENT WHEN READY AT COMP
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