#pragma once

#include "api.h"
#include "lemlib/api.hpp"

extern pros::Controller Controller;

// Drivetrain
extern pros::MotorGroup LeftMotors;
extern pros::MotorGroup RightMotors;
extern lemlib::Drivetrain Drivetrain;
extern lemlib::ExpoDriveCurve ThrottleCurve;
extern lemlib::Chassis Chassis;

// Pneumatics
extern pros::adi::DigitalOut ClampPort;
extern pros::adi::DigitalOut DoinkerPort;

// Gearbox
extern pros::Motor TopMotor;
extern pros::Motor BottomMotor;
extern pros::Rotation ArmSensor;
extern pros::Controller Controller;

// Sensors
extern pros::Optical OpticalSensor;
extern pros::Imu Imu;
extern pros::Rotation VerticalTracking;
extern lemlib::TrackingWheel VerticaTrackingWheel;
extern lemlib::OdomSensors Sensors;

// PID 
extern lemlib::ControllerSettings LateralController;
extern lemlib::ControllerSettings AngularController;