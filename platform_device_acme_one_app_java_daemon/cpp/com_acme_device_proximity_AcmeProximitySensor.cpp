#include <jni.h>
#include <string>
#include <hardware/hardware.h>

#include "dev/proximity_hal.h"

JNIEXPORT jlong JNICALL Java_com_acme_device_proximity_AcmeProximitySensor_open
  (JNIEnv *env, jclass clazz) {
    const hw_module_t *module;

    if (hw_get_module(ACME_PROXIMITY_SENSOR_MODULE, &module))
        return -1;

    long device;
    if (module->methods->open(
            module,
            nullptr,
            reinterpret_cast<struct hw_device_t **>(&device)))
        return -1;

    return (jlong) device;
}

JNIEXPORT jint JNICALL Java_com_acme_device_proximity_AcmeProximitySensor_poll
  (JNIEnv *env, jclass clazz, jlong handle, jint precision) {
    auto *device = reinterpret_cast<proximity_sensor_device_t *>(handle);
    return device->poll_sensor(device, precision);
}

JNIEXPORT jint JNICALL Java_com_acme_device_proximity_AcmeProximitySensor_close
  (JNIEnv *env, jclass clazz, jlong handle) {
    auto device = reinterpret_cast<proximity_sensor_device_t *>(handle);
    return device->common.close(reinterpret_cast<hw_device_t *>(device));
}
