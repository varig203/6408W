#pragma once

#include "api.h"
#include "lemlib/api.hpp"

// Drivetrain
extern pros::MotorGroup left_motors;
extern pros::MotorGroup right_motors;
extern lemlib::Drivetrain drivetrain;
extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::Chassis chassis;

// Pneumatics
extern pros::adi::DigitalOut clamp_port;
extern pros::adi::DigitalOut doinker_port;

// Gearbox
extern pros::Motor TopMotor;
extern pros::Motor BottomMotor;
extern pros::Rotation Arm_Sensor;
extern pros::Controller controller;

// Sensors
extern pros::Optical Optical_Sensor;
extern pros::Imu imu;
extern pros::Rotation VerticalTracking;
extern lemlib::TrackingWheel vertical_tracking_wheel;
extern lemlib::OdomSensors sensors;

// PID 
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;