#include <unistd.h>
#include <stdio.h>
#include <android/log.h>
#include <hardware/hardware.h>

#include "dev/proximity_hal.h"

#define DELAY_SECS 60
#define ALOG(msg) __android_log_write(ANDROID_LOG_DEBUG, "PROXIMITYD", msg)

int main(int argc, char *argv[]) {
    const hw_module_t* module;
    if (hw_get_module(ACME_PROXIMITY_SENSOR_MODULE, &module)) {
        ALOG("Failed to load the Acme proximity HAL module");
        return -1;
    }

    proximity_sensor_device_t* device;
    if (module->methods->open(
        module,
        nullptr,
        reinterpret_cast<struct hw_device_t**>(&device))) {
        ALOG("Failed to open Acme proximity HAL");
    	return -1;
    }

    proximity_params_t* config = &(device->params);
    char message[128];

    int n = 0;
    int precision;

    while (true) {
        sleep(DELAY_SECS);

        n++;
        if (n < 10) {
            precision = 40;
        } else {
            n = 0;
            precision = 80;
        }

        int proximity = device->poll_sensor(device, precision);

        if ((proximity < config->proximity.min)) {
            device->common.close(reinterpret_cast<hw_device_t *>(device));
            return 0;
        }

        snprintf(message, sizeof(message), "proximity @%2d: %4.2f", precision,
            (100.0 * (proximity - config->proximity.min)) / config->proximity.range);
        ALOG(message);
    } 
}
