#include "lemlib/chassis/chassis.hpp"
#include "main.h"

// ---- Controller ----
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// ---- Drive Motors ----
pros::MotorGroup LeftMotors({5, -6, 7}, pros::MotorGearset::blue);
pros::MotorGroup RightMotors({-1, 3, -4}, pros::MotorGearset::blue);

// ---- Gearbox/Intake ----
pros::Motor TopMotor(9);
pros::Motor BottomMotor(18);
pros::Rotation ArmSensor(8);
pros::Optical OpticalSensor(12);

// ---- Pneumatics ----
pros::adi::DigitalOut ClampPort('H');
pros::adi::DigitalOut DoinkerPort('G');

// ---- Sensors ----
pros::Rotation VerticalTracking(-16);
pros::Imu Imu(19);
lemlib::TrackingWheel VerticalTrackingWheel(&VerticalTracking, lemlib::Omniwheel::NEW_2, 1);
lemlib::OdomSensors sensors(&VerticalTrackingWheel, nullptr, nullptr, nullptr, &Imu);

// ---- Drivetrain PID ----
// https://lemlib.readthedocs.io/en/stable/tutorials/4_pid_tuning.html
// ^ PID Tuning Guide
lemlib::ControllerSettings LateralController(20, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              170, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

lemlib::ControllerSettings AngularController(1.65, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              12, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// ---- Defining drive ----
lemlib::ExpoDriveCurve ThrottleCurve(3, // joystick deadband out of 127
                                    10, // minimum output where drivetrain will move out of 127
                                        2 // expo curve gain
);
lemlib::ExpoDriveCurve SteerCurve(3,
                                 10,
                                     2
);
lemlib::Drivetrain drivetrain(&RightMotors, &LeftMotors,11.5, lemlib::Omniwheel::NEW_325, 450, 2);
lemlib::Chassis chassis(drivetrain, LateralController, AngularController, sensors, &ThrottleCurve);
