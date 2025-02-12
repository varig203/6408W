#ifndef _GEARBOX_HPP_
#define _GEARBOX_HPP_

#include "main.h"
#include "lemlib/api.hpp"

// Function declarations
void StopAll();
void Intake();
void MarryUp_and_FullOut();
void Load_Arm_PID();
void Kill_Arm();
void Controll_Gears();

// Declare the motors as extern so they can be configured in main.cpp
extern pros::Motor TopMotor;
extern pros::Motor BottomMotor;
extern pros::Rotation Arm_Sensor;

#endif // _GEARBOX_HPP_ 