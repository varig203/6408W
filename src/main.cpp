#include "main.h"
#include "pnuematic.hpp"  // Include the declaration for clamp_fn and doinker
#include "gearbox.hpp"    // Include the declaration for Controll_Gears    // Include the declaration for intake
#include "autons.hpp"
#include "pros/rtos.hpp"
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
	{"Skills", Large_Balls_To_Lick},
});

rd::Console Console; // Creating the terminal
rd::Image Image1(&team_logo,"Whopper"); // Creating the image

// Creating tasks
void chassis_fn() { 
	while (true) {
		// Get throttle and turning values from the controller.
		int throttle = Controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = Controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		//Chassis.curvature(-throttle, turn, false); // UNCOMMENT TO TEST CHEESY DRIVE
		Chassis.arcade(-throttle, turn, false, 0.75);
		pros::delay(20);
	}
}

void pneumatics_fn() {
	bool doinkerPrev = false;
    bool clampPrev = false;
	
	while (true) {
		bool doinkerCurr = Controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		if (doinkerCurr && !doinkerPrev) {
			doinker_fn();  // Toggle the G port by calling the doinker function
		}
		doinkerPrev = doinkerCurr;

		bool clampCurr = Controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);
		if (clampCurr && !clampPrev) {
			clamp_fn();  // Toggle the H port by calling the clamp function
		}
		clampPrev = clampCurr;

		pros::delay(20);
	}
}

// Runs initialization code when the program starts; all other competition modes are blocked, keep exec under few secs
void initialize() {
	// Set up drivetrain brake modes
	LeftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	RightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	// Initalize gearbox and calibrate drive sensors
	initializeGearbox_fn();
	Chassis.calibrate();

	// Focus onto auton selector
	autonSelector.focus();
}


// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {}

// Runs after initalize and before auton. only when connected to field control
void competition_initialize() {}

// auton
void autonomous() {
	//Large_Balls_To_Lick(); // skills 
	autonSelector.run_auton(); // REMOVE COMMENT WHEN READY AT COMP
}

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
	// Calls all the threads to start
	pros::Task chassis_task(chassis_fn);
	pros::Task pneumatics_task(pneumatics_fn);
	// Put here as it wasn't working
	while (true) {
		GearBox_Control();

		pros::delay(25);
	}
}