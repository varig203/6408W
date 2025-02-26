#include "main.h"  // Ensure all PROS definitions are available
#include "pnuematic.hpp"

/*****************************************************
 *                       NOTE:                       *
 * SOLENOIDS CANNOT SEND DATA AND CAN ONLY READ DATA *
 *****************************************************/

void clamp_fn(void) {
    // maintain a static state flag in this function:
    static bool clamp_state = false;
    clamp_state = !clamp_state;
    clamp_port.set_value(clamp_state);
}

void doinker(void) {
    // Maintain a static state flag for the G port
    static bool doinker_state = false;
    doinker_state = !doinker_state;
    doinker_port.set_value(doinker_state);
}