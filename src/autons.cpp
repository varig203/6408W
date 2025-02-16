#include "main.h"
#include "pnuematic.hpp"  // Include the declaration for clamp_fn and doinker
#include "../include/gearbox.hpp"    // Include the declaration for Controll_Gears    // Include the declaration for intake
#include "lemlib/api.hpp"  // Include lemlib API for chassis and drivetrain

void Blue_Side_Auton() {
    chassis.setPose(0,0,0);
	clamp_fn();
	chassis.moveToPose(0,24,0,1550);
	pros::delay(1000);
	clamp_fn();
    pros::delay(1000);
    intake();
    pros::delay(1000);
    stop_intake();
}

void Red_Side_Auton() {
    // Your red autonomous code here
}