#ifndef ACME_PROXIMITY_SENSOR_H
#define ACME_PROXIMITY_SENSOR_H

#include "proximity_hal.h"

int open_sensor(proximity_params_t &params);

int poll_sensor(int fd, int precision);

int close_sensor(int fd);

#endif //ACME_PROXIMITY_SENSOR_H

