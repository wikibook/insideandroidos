#ifndef PROXIMITY_HAL_H
#define PROXIMITY_HAL_H

#include <hardware/hardware.h>

#define ACME_PROXIMITY_SENSOR_MODULE "proximityhal"

typedef struct proximity_sensor_device proximity_sensor_device_t;

struct value_range {
    int min;
    int range;
};

typedef struct proximity_params {
    struct value_range precision;
    struct value_range proximity;
} proximity_params_t;

struct proximity_sensor_device {
    hw_device_t common;

    int fd;

    proximity_params_t params;

    int (*poll_sensor)(proximity_sensor_device_t *dev, int precision);
};

#endif // PROXIMITY_HAL_H

