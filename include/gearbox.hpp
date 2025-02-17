#ifndef GEARBOX_HPP
#define GEARBOX_HPP

// Declaration for gearbox control functions

// Runs the gearbox control loop that handles the intake, PID arm load, and outtake functions.
void GearBox_Control();
void intake();
void raise_arm();
void stop_intake();
void auton_intake();
void initialize_gearbox();

#endif // GEARBOX_HPP 