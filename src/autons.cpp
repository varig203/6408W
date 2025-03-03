#include "main.h"
#include "pnuematic.hpp"
#include "gearbox.hpp"

// we should revamp autos by friday
void Blue_Side_Auton() {
    Chassis.setPose(0,0,0);
	clamp_fn();
	Chassis.moveToPose(0,38,0, 1000, {.maxSpeed = 90}); // go north to get angle on fisrt ring stack
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	autonIntake_fn();
	pros::delay(500);
	Chassis.moveToPoint(5,40,1000, {.forwards = false});
	pros::delay(500);
	Chassis.moveToPoint(15,43,2000, {.forwards = false, .maxSpeed = 90});    
	Chassis.moveToPoint(35,45,2000, {.forwards = false, .maxSpeed = 90});
	Chassis.moveToPoint(25,43,1000); // avoid intaking red ring by backing up
	Chassis.moveToPoint(25,35,2000, {.forwards = false});
	pros::delay(1000);
	Chassis.turnToPoint(17,30,500, {.forwards = false});
	Chassis.moveToPoint(17,35,2000, {.forwards = false}, true); // backwards to intake last ring
	pros::delay(2000); // time to finish intaking last ring
	stopIntake_fn();
	Chassis.moveToPoint(-18,30,2000, {.forwards = false, .maxSpeed = 80});
}
 
// exact mirror of blue side (change x to -x and rotate turns by 180)
void Red_Side_Auton() {
    Chassis.setPose(0,0,0);
	clamp_fn();
	Chassis.moveToPose(0,38,0, 1000, {.maxSpeed = 90}); // go north to get angle on fisrt ring stack
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	autonIntake_fn();
	pros::delay(500);
	Chassis.moveToPoint(-5,40,1000, {.forwards = false});
	pros::delay(500);
	Chassis.moveToPoint(-15,43,2000, {.forwards = false, .maxSpeed = 90});    
	Chassis.moveToPoint(-35,45,2000, {.forwards = false, .maxSpeed = 90});
	Chassis.moveToPoint(-25,43,1000); // avoid intaking red ring by backing up
	Chassis.moveToPoint(-25,35,2000, {.forwards = false});
	pros::delay(1000);
	Chassis.turnToPoint(-17,30,500, {.forwards = false});
	Chassis.moveToPoint(-17,35,2000, {.forwards = false}, true); // backwards to intake last ring
	pros::delay(2000); // time to finish intaking last ring
	stopIntake_fn();
	Chassis.moveToPoint(18,30,2000, {.forwards = false, .maxSpeed = 80});
}

// probably want to change these to try and rush (end of thursday)
void Blue_Goal_Auton() {
    Chassis.setPose(0,0,0);
	clamp_fn();
	Chassis.moveToPose(0, 28, 0, 3000); // north to first mogo
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	autonIntake_fn();
	Chassis.turnToHeading(90, 500);
	pros::delay(1000);
	Chassis.moveToPoint(-24, 32, 2000, {.forwards = false}, true);
	pros::delay(2000);
	stopIntake_fn();
    Chassis.moveToPoint(14, 40, 2000, {.forwards = false}, true);
}
void Red_Goal_Auton() {
    Chassis.setPose(0,0,0);
	clamp_fn();
	Chassis.moveToPose(0, 28, 0, 3000);
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	autonIntake_fn();
	Chassis.turnToHeading(270, 500);
	pros::delay(1000);
	Chassis.moveToPoint(24, 32, 2000, {.forwards = false}, true);
	pros::delay(2000);
	stopIntake_fn();
    Chassis.moveToPoint(-14, 40, 2000, {.forwards = false}, true);
}
void universal_auton() {
    Chassis.setPose(0,0,0);
	clamp_fn();
	Chassis.moveToPose(0, 28, 0, 3000);
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	autonIntake_fn();
    pros::delay(2000);
    stopIntake_fn();
}

void Large_Balls_To_Lick() { // Creating for now for auton selector
	Chassis.setPose(0,0,180);
	autonIntake_fn();
	pros::delay(500);
	clamp_fn();
	pros::delay(100);
	stopIntake_fn();
	Chassis.moveToPoint(0,12,2000, {.forwards = false});
	Chassis.turnToPoint(19,12,1000, {.maxSpeed = 80});
	Chassis.moveToPoint(19,12,2000, {.maxSpeed = 60});
	pros::delay(1000);
	clamp_fn();
	pros::delay(500);
	autonIntake_fn();
	Chassis.moveToPoint(22,25,1000,{.forwards = false, .maxSpeed = 90});
	Chassis.moveToPoint(60,61,3000, {.forwards = false, .maxSpeed = 90});
	pros::delay(500);
	Chassis.turnToPoint(56,37,1000, {.forwards = false});
	Chassis.moveToPoint(56,37,1000, {.forwards = false});
	Chassis.moveToPoint(56,0,4000,{.forwards = false, .maxSpeed = 50});
	Chassis.moveToPoint(50,10,1000);
	Chassis.turnToPoint(63,13,500,{.forwards = false, .maxSpeed = 90});
	pros::delay(500);
	Chassis.moveToPoint(63,13,1000, {.forwards = false, .maxSpeed = 80});
	pros::delay(500);
	Chassis.moveToPoint(69,-3,1000);
	Chassis.turnToPoint(22,25,500,{.forwards = false});
	pros::delay(500);
	stopIntake_fn();
	pros::delay(50);
	clamp_fn();
	pros::delay(50);
	raiseArm_fn();
	pros::delay(250);
	autonIntake_fn();
	pros::delay(500);
	stopIntake_fn();
	Chassis.moveToPoint(19,13,3000);
	Chassis.turnToPoint(-19,10,500,{.maxSpeed = 90});
	Chassis.moveToPoint(-19,10,2000,{.maxSpeed = 50});
	pros::delay(1250); // change to save time
	clamp_fn();
	// end of q1
	// adjust q2 to account for tracking error
	pros::delay(500);
	autonIntake_fn();
	Chassis.moveToPoint(-15,25,1000,{.forwards = false, .maxSpeed = 90});
	Chassis.moveToPoint(-53,61,3000, {.forwards = false, .maxSpeed = 90});
	pros::delay(500);
	Chassis.turnToPoint(-45,37,1000, {.forwards = false});
	Chassis.moveToPoint(-45,37,1000, {.forwards = false});
	Chassis.moveToPoint(-45,0,4000,{.forwards = false, .maxSpeed = 50});
	Chassis.moveToPoint(-45,10,1000);
	Chassis.turnToPoint(-53,13,500,{.forwards = false, .maxSpeed = 90});
	pros::delay(500);
	Chassis.moveToPoint(-55,14,1000, {.forwards = false, .maxSpeed = 80});
	pros::delay(500);
	Chassis.moveToPoint(-67,-3,1000);
	Chassis.turnToPoint(-22,25,500,{.forwards = false});
	pros::delay(500);
	stopIntake_fn();
	pros::delay(50);
	clamp_fn();
	pros::delay(50);
	raiseArm_fn();
	pros::delay(250);
	autonIntake_fn();
	pros::delay(500); 
	stopIntake_fn();
	// end of q2
	// go for third mogo
	Chassis.moveToPoint(-47,61,2000,{.forwards = false, .maxSpeed = 90});
	autonIntake_fn();
	Chassis.moveToPoint(-48,78,1000,{.forwards = false});
	pros::delay(1600);
	stopIntake_fn();
	Chassis.moveToPoint(-40,93,1000,{.forwards = false});
	Chassis.turnToPoint(-3,107,1000);
	Chassis.moveToPoint(-3,107,2000,{.maxSpeed = 65});
	pros::delay(1250);
	clamp_fn();
	pros::delay(500);
	autonIntake_fn();
	Chassis.moveToPoint(-19,78,2000,{.forwards = false});
	Chassis.moveToPoint(-38,78,1000,{.forwards = false});
	Chassis.turnToPoint(-44.5,122,1000,{.forwards = false});
	Chassis.moveToPoint(-44.5,122,2000,{.forwards = false, .maxSpeed = 70});
	Chassis.moveToPoint(-40,90,1000);
	Chassis.turnToPoint(-55,107,1000,{.forwards = false, .maxSpeed = 90});
	Chassis.moveToPoint(-55,107,1000,{.forwards = false, .maxSpeed = 80});
	pros::delay(2500);
	stopIntake_fn();
	Chassis.moveToPoint(-45,90,1000);
	Chassis.turnToPoint(-63,135,1000);
	Chassis.moveToPoint(-63,135,1000);
	pros::delay(250);
	clamp_fn();
	pros::delay(250);
	raiseArm_fn();
	pros::delay(500);
	autonIntake_fn();
	pros::delay(750);
	stopIntake_fn();
	Chassis.moveToPoint(-55,121,500,{.forwards = false});
	Chassis.moveToPoint(0,121,4000,{.forwards = false});
	Chassis.moveToPoint(70,121,3000,{.forwards = false});
	Chassis.moveToPoint(30,100,1000);
} 

// REMEMBER TO NEVER MAKE TASKS SHORT LIVED
void test(){
    // Now we can specify a custom timeout (e.g., 3000ms = 3 seconds)
    pros::Task IntakeTask([]() { autonIntakeHandler_fn(3000); }, "Intake Task");
    Chassis.moveToPose(0,10,0,500);
}
