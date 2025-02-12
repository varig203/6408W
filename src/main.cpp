#include "main.h"
#include "pnuematic.hpp"  // Include the declaration for clamp_fn and doinker
#include "../include/gearbox.hpp"    // Include the declaration for Controll_Gears

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

	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {}

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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({5, -6, 7});    // Left motors: forward port 5, reversed port 6, forward port 7
	pros::MotorGroup right_mg({-1, 3, -4});   // Right motors: reversed port 1, forward port 3, reversed port 4

	// New constants for exponential drive with rescaled deadband and cubic mix
	const double EXPO_PARAMETER = 0.5;  // 0 => fully linear, 1 => fully cubic (increased for more expo effect)
	const int DEADBAND = 2;             // Further reduced deadband for finer control at very low inputs

    // Lambda to apply expo drive (with deadband compensation and remapping)
    auto applyExpo = [=](int input) -> int {
        if (abs(input) < DEADBAND)
            return 0;
        double sign = (input >= 0) ? 1.0 : -1.0;
        // Remap input so that after deadband, full range [0, 1] is used
        double normalized = (double)(abs(input) - DEADBAND) / (127 - DEADBAND);
        // Apply a mix of linear and cubic response for exponential drive
        double expo_val = (1 - EXPO_PARAMETER) * normalized + EXPO_PARAMETER * normalized * normalized * normalized;
        return static_cast<int>(expo_val * 127 * sign);
    };

    // Variables to track previous states of the A and B buttons
    bool a_button_prev = false;
    bool b_button_prev = false;

	while (true) {
		// Check if the A button was just pressed (edge detection)
		bool a_button_curr = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		if (a_button_curr && !a_button_prev) {
			doinker();  // Toggle the G port by calling the doinker function
		}
		a_button_prev = a_button_curr;

		// Check if the B button was just pressed (edge detection)
		bool b_button_curr = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
		if (b_button_curr && !b_button_prev) {
			clamp_fn();  // Toggle the H port by calling the clamp function
		}
		b_button_prev = b_button_curr;

		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		// Get raw joystick values
		int forward = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_RIGHT_X);

		// Process values through the expo function
		int forward_expo = applyExpo(forward);
		int turn_expo = applyExpo(turn);

		// Calculate motor powers
		int left_power = forward_expo - turn_expo;
		int right_power = forward_expo + turn_expo;

		// Apply motor power
		left_mg.move(left_power);
		right_mg.move(right_power);

		// Run gearbox control
		Controll_Gears();

		pros::delay(20); // Run for 20 ms then update
	}
}