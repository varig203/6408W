#include "main.h"  // Ensure all PROS definitions are available
#include "pnuematic.hpp"

/*****************************************************
 *                       NOTE:                       *
 * SOLENOIDS CANNOT SEND DATA AND CAN ONLY READ DATA *
 *****************************************************/

// Create a static digital output instance for port 'H' using the updated type
static pros::adi::DigitalOut clamp_port('H');

void clamp_fn(void) {
    // maintain a static state flag in this function:
    static bool clamp_state = false;
    clamp_state = !clamp_state;
    clamp_port.set_value(clamp_state);
}

// Create a static digital output instance for port 'G' using the updated type
static pros::adi::DigitalOut doinker_port('G');

void doinker(void) {
    // Maintain a static state flag for the G port
    static bool doinker_state = false;
    doinker_state = !doinker_state;
    doinker_port.set_value(doinker_state);
}