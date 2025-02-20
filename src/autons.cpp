#include "main.h"
#include "pnuematic.hpp"  // Include the declaration for clamp_fn and doinker
#include "../include/gearbox.hpp"    // Include the declaration for Controll_Gears    // Include the declaration for intake
#include "lemlib/api.hpp"  // Include lemlib API for chassis and drivetrain

// we should revamp autos by friday
void Blue_Side_Auton() {
    chassis.setPose(0,0,0);
	clamp_fn();
	chassis.moveToPose(0, 28, 0, 3000); // mogo 1 pos
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	auton_intake();
	chassis.moveToPose(0,35,0, 1000); // go north to get angle on fisrt ring stack
	chassis.turnToHeading(270, 500);
	pros::delay(1000);
	chassis.moveToPoint(24,32,2000, {.forwards = false}, true); // backwards to intake first ring
	pros::delay(500);
	chassis.moveToPoint(35,47,2000, {.forwards = false}, true); // turn towards 2 rings in feild and star arc
	chassis.turnToHeading(90,1000);
	pros::delay(1000);
	chassis.moveToPoint(8,50.5,2000, {.forwards = false,.maxSpeed = 60}, true); // finish ark at the tower
	pros::delay(3000); // change to stop intaking opponent top ring
	stop_intake();
}

// exact mirror of blue side (change x to -x and rotate turns by 180)
void Red_Side_Auton() {
    chassis.setPose(0,0,0);
	clamp_fn();
	chassis.moveToPose(0, 28, 0, 3000);
	pros::delay(1000);
	clamp_fn();
	pros::delay(250);
	auton_intake();
	chassis.moveToPose(0,35,0, 1000);
	chassis.turnToHeading(90, 500);
	pros::delay(1000);
	chassis.moveToPoint(-24,32,2000, {.forwards = false}, true);
	pros::delay(500);
	chassis.moveToPoint(-35,48,2000, {.forwards = false}, true);
	chassis.turnToHeading(270,1000);
	pros::delay(1000);
	chassis.moveToPoint(-8,50.5,2000, {.forwards = false,.maxSpeed = 60}, true);
	pros::delay(3000);
	stop_intake();
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

void test(){
    // Now we can specify a custom timeout (e.g., 3000ms = 3 seconds)
    pros::Task IntakeTask([]() { auton_intake_handler(3000); }, "Intake Task");
    chassis.moveToPose(0,10,0,500);
}
