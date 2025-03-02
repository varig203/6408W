#pragma once

// Declaration for gearbox control functions

// Runs the gearbox control loop that handles the intake, PID arm load, and outtake functions.
void GearBox_Control();
void intake_fn();
void raiseArm_fn();
void stopIntake_fn();
void autonIntake_fn();
void initializeGearbox_fn();
void autonIntakeHandler_fn(int timeout_ms);
