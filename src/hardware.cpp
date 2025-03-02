#include "main.h"

// ---- Controller ----
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// ---- Drive Motors ----
pros::MotorGroup left_motors({5, -6, 7}, pros::MotorGearset::blue);
pros::MotorGroup right_motors({-1, 3, -4}, pros::MotorGearset::blue);

// ---- Gearbox/Intake ----
pros::Motor TopMotor(9);
pros::Motor BottomMotor(18);
pros::Rotation Arm_Sensor(8);
pros::Optical Optical_Sensor(12);

// ---- Pneumatics ----
pros::adi::DigitalOut clamp_port('H');
pros::adi::DigitalOut doinker_port('G');

// ---- Sensors ----
pros::Rotation VerticalTracking(-16);
pros::Imu imu(19);
lemlib::TrackingWheel vertical_tracking_wheel(&VerticalTracking, lemlib::Omniwheel::NEW_2, 1);
lemlib::OdomSensors sensors(&vertical_tracking_wheel, nullptr, nullptr, nullptr, &imu);

// ---- Drivetrain PID ----
// https://lemlib.readthedocs.io/en/stable/tutorials/4_pid_tuning.html
// ^ PID Tuning Guide
lemlib::ControllerSettings lateral_controller(20, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              170, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(1.65, // proportional gain (kP)
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
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     2 // expo curve gain
);
lemlib::Drivetrain drivetrain(&right_motors, &left_motors,11.5, lemlib::Omniwheel::NEW_325, 450, 2);
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);
