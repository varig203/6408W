#include "main.h"
#include "pnuematic.hpp"  // Include the declaration for clamp_fn and doinker
#include "gearbox.hpp"    // Include the declaration for Controll_Gears    // Include the declaration for intake

// we should revamp autos by friday
void Blue_Side_Auton() {
    chassis.setPose(0,0,0);
	clamp_fn();
	chassis.moveToPose(0,38,0, 1000, {.maxSpeed = 90}); // go north to get angle on fisrt ring stack
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	auton_intake();
	pros::delay(500);
	chassis.moveToPoint(5,40,1000, {.forwards = false});
	pros::delay(500);
	chassis.moveToPoint(15,43,2000, {.forwards = false, .maxSpeed = 90});    
	chassis.moveToPoint(35,45,2000, {.forwards = false, .maxSpeed = 90});
	chassis.moveToPoint(25,43,1000); // avoid intaking red ring by backing up
	chassis.moveToPoint(25,35,2000, {.forwards = false});
	pros::delay(1000);
	chassis.turnToPoint(17,30,500, {.forwards = false});
	chassis.moveToPoint(17,35,2000, {.forwards = false}, true); // backwards to intake last ring
	pros::delay(2000); // time to finish intaking last ring
	stop_intake();
	chassis.moveToPoint(-18,30,2000, {.forwards = false, .maxSpeed = 80});
}
 
// exact mirror of blue side (change x to -x and rotate turns by 180)
void Red_Side_Auton() {
    chassis.setPose(0,0,0);
	clamp_fn();
	chassis.moveToPose(0,38,0, 1000, {.maxSpeed = 90}); // go north to get angle on fisrt ring stack
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	auton_intake();
	pros::delay(500);
	chassis.moveToPoint(-5,40,1000, {.forwards = false});
	pros::delay(500);
	chassis.moveToPoint(-15,43,2000, {.forwards = false, .maxSpeed = 90});    
	chassis.moveToPoint(-35,45,2000, {.forwards = false, .maxSpeed = 90});
	chassis.moveToPoint(-25,43,1000); // avoid intaking red ring by backing up
	chassis.moveToPoint(-25,35,2000, {.forwards = false});
	pros::delay(1000);
	chassis.turnToPoint(-17,30,500, {.forwards = false});
	chassis.moveToPoint(-17,35,2000, {.forwards = false}, true); // backwards to intake last ring
	pros::delay(2000); // time to finish intaking last ring
	stop_intake();
	chassis.moveToPoint(18,30,2000, {.forwards = false, .maxSpeed = 80});
}

// probably want to change these to try and rush (end of thursday)
void Blue_Goal_Auton() {
    chassis.setPose(0,0,0);
	clamp_fn();
	chassis.moveToPose(0, 28, 0, 3000); // north to first mogo
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	auton_intake();
	chassis.turnToHeading(90, 500);
	pros::delay(1000);
	chassis.moveToPoint(-24, 32, 2000, {.forwards = false}, true);
	pros::delay(2000);
	stop_intake();
    chassis.moveToPoint(14, 40, 2000, {.forwards = false}, true);
}
void Red_Goal_Auton() {
    chassis.setPose(0,0,0);
	clamp_fn();
	chassis.moveToPose(0, 28, 0, 3000);
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	auton_intake();
	chassis.turnToHeading(270, 500);
	pros::delay(1000);
	chassis.moveToPoint(24, 32, 2000, {.forwards = false}, true);
	pros::delay(2000);
	stop_intake();
    chassis.moveToPoint(-14, 40, 2000, {.forwards = false}, true);
}
void universal_auton() {
    chassis.setPose(0,0,0);
	clamp_fn();
	chassis.moveToPose(0, 28, 0, 3000);
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	auton_intake();
    pros::delay(2000);
    stop_intake();
}

void skills() { // Creating for now for auton selector
	chassis.setPose(0,0,180);
	auton_intake();
	pros::delay(500);
	clamp_fn();
	pros::delay(100);
	stop_intake();
	chassis.moveToPoint(0,13,2000, {.forwards = false});
	chassis.turnToPoint(19,13,1000, {.maxSpeed = 80});
	chassis.moveToPoint(19,13,2000, {.maxSpeed = 60});
	pros::delay(1000);
	clamp_fn();
	pros::delay(500);
	auton_intake();
	chassis.moveToPoint(22,25,1000,{.forwards = false, .maxSpeed = 90});
	chassis.moveToPoint(60,61,3000, {.forwards = false, .maxSpeed = 90});
	pros::delay(500);
	chassis.turnToPoint(56,37,1000, {.forwards = false});
	chassis.moveToPoint(56,37,1000, {.forwards = false});
	chassis.moveToPoint(56,0,4000,{.forwards = false, .maxSpeed = 50});
	chassis.moveToPoint(50,10,1000);
	chassis.turnToPoint(63,13,500,{.forwards = false, .maxSpeed = 90});
	pros::delay(500);
	chassis.moveToPoint(63,13,1000, {.forwards = false, .maxSpeed = 80});
	pros::delay(500);
	chassis.moveToPoint(69,-3,1000);
	chassis.turnToPoint(22,25,500,{.forwards = false});
	pros::delay(500);
	stop_intake();
	pros::delay(250);
	clamp_fn();
	pros::delay(250);
	raise_arm();
	pros::delay(500);
	auton_intake();
	pros::delay(750);
	stop_intake();
	chassis.moveToPoint(19,13,3000);
	chassis.turnToPoint(-19,13,500,{.maxSpeed = 90});
	chassis.moveToPoint(-19,13,2000,{.maxSpeed = 50});
	pros::delay(1250); // change to save time
	clamp_fn();
	// end of q1
	// adjust q2 to account for tracking error
	pros::delay(500);
	auton_intake();
	chassis.moveToPoint(-15,25,1000,{.forwards = false, .maxSpeed = 90});
	chassis.moveToPoint(-48,61,3000, {.forwards = false, .maxSpeed = 90});
	pros::delay(500);
	chassis.turnToPoint(-45,37,1000, {.forwards = false});
	chassis.moveToPoint(-45,37,1000, {.forwards = false});
	chassis.moveToPoint(-45,0,4000,{.forwards = false, .maxSpeed = 50});
	chassis.moveToPoint(-45,10,1000);
	chassis.turnToPoint(-53,13,500,{.forwards = false, .maxSpeed = 90});
	pros::delay(500);
	chassis.moveToPoint(-53,13,1000, {.forwards = false, .maxSpeed = 80});
	pros::delay(500);
	chassis.moveToPoint(-67,-3,1000);
	chassis.turnToPoint(-22,25,500,{.forwards = false});
	pros::delay(500);
	stop_intake();
	pros::delay(250);
	clamp_fn();
	pros::delay(250);
	raise_arm();
	pros::delay(500);
	auton_intake();
	pros::delay(750);
	stop_intake();
	// end of q2
	// go for third mogo
	

} 

// REMEMBER TO NEVER MAKE TASKS SHORT LIVED
void test(){
    // Now we can specify a custom timeout (e.g., 3000ms = 3 seconds)
    pros::Task IntakeTask([]() { auton_intake_handler(3000); }, "Intake Task");
    chassis.moveToPose(0,10,0,500);
}
