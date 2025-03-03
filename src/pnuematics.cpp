#include "main.h"  // Ensure all PROS definitions are available
#include "pnuematic.hpp"

/*****************************************************
 *                       NOTE:                       *
 * SOLENOIDS CANNOT SEND DATA AND CAN ONLY READ DATA *
 *****************************************************/

void clamp_fn(void) {
    static bool clampState = false;
    clampState = !clampState;
    ClampPort.set_value(clampState);
}

void doinker_fn(void) {
    static bool doinkerState = false;
    doinkerState = !doinkerState;
    DoinkerPort.set_value(doinkerState);
}