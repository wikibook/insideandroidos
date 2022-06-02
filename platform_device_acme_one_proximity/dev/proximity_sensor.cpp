#include "dev/proximity_sensor.h"

// This is stub, mocking actual glue code.
// If this were a real thing, it would talk to a device driver,
// presumably for a USB device

int open_sensor(proximity_params_t &params) {
    params.precision.min = 0;
    params.precision.range = 100;
    params.proximity.min = 0;
    params.proximity.range = 100;

    return 0; // a completely fake fd
}

int poll_sensor(int fd, int precision) {
    if (precision < 0) {
        return -1;
    } else if (precision < 70) {
        return 60;
    } else if (precision < 100) {
        return 63;
    } else {
        return -1;
    }
}

int close_sensor(int fd) {
    return 0;
}

