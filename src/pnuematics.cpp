#include "main.h"  // Ensure all PROS definitions are available
#include "pnuematic.hpp"

/*****************************************************
 *                       NOTE:                       *
 * SOLENOIDS CANNOT SEND DATA AND CAN ONLY READ DATA *
 *****************************************************/

void clamp_fn(void) {
    static bool clamp_state = false;
    clamp_state = !clamp_state;
    ClampPort.set_value(clamp_state);
}

void doinker_fn(void) {
    static bool doinker_state = false;
    doinker_state = !doinker_state;
    DoinkerPort.set_value(doinker_state);
}