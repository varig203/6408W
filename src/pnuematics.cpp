#include "main.h"  // Ensure all PROS definitions are available
#include "pnuematic.hpp"
#include "pros/adi.hpp"  // still needed for pros::adi::DigitalOut

// Create a static digital output instance for port 'H' using the updated type
static pros::adi::DigitalOut clamp_port('H');

void clamp_fn(void) {
    // Instead of reading the state from the port (which is now inaccessible),
    // maintain a static state flag in this function:
    static bool clamp_state = false;
    clamp_state = !clamp_state;
    clamp_port.set_value(clamp_state);
}