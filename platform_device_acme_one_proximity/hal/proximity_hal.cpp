#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <log/log_system.h>

#include "dev/proximity_hal.h"
#include "dev/proximity_sensor.h"

#define LOG_TAG "PROX"

static int poll_proximity_sensor(proximity_sensor_device_t *dev, int precision) {
    SLOGV("Polling proximity sensor");

    if (!dev) {
        SLOGV("No device provided");
        return -1;
    }

    return poll_sensor(dev->fd, precision);
}

static int close_proximity_sensor(proximity_sensor_device_t *dev) {
    SLOGV("Closing proximity sensor");

    if (!dev)
        return 0;

    close_sensor(dev->fd);
    free(dev);
    return 0;
}

static int open_proximity_sensor_module(
        const struct hw_module_t *module,
        char const *name,
        struct hw_device_t **device) {
    SLOGV("Opening proximity sensor");

    auto *dev = static_cast<proximity_sensor_device_t*>
      (malloc(sizeof(proximity_sensor_device_t)));
    if (!dev)
        return -ENOMEM;

    memset(dev, 0, sizeof(*dev));

    int fd = open_sensor(dev->params);
    if (fd < 0) {
        SLOGE("Failed to open proximity sensor: %s", strerror(errno));
        free(dev);
        return -1;
    }
    dev->fd = fd;

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t *) module;
    dev->common.close = (int (*)(struct hw_device_t *)) close_proximity_sensor;

    dev->poll_sensor = poll_proximity_sensor;

    *device = reinterpret_cast<hw_device_t *>(dev);

    return 0;
}

static hw_module_methods_t proximity_sensor_methods = {
        .open = open_proximity_sensor_module
};

hw_module_t HAL_MODULE_INFO_SYM = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = HARDWARE_HAL_API_VERSION,
        .hal_api_version = 0,
        .id = ACME_PROXIMITY_SENSOR_MODULE,
        .name = "Acme Proximity Sensor",
        .author = "Acme Team",
        .methods = &proximity_sensor_methods
};

