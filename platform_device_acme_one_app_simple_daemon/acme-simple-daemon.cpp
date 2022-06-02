#include <unistd.h>
#include <stdio.h>
#include <android/log.h>
#include "dev/proximity_sensor.h"

#define DELAY_SECS 60
#define ALOG(msg) __android_log_write(ANDROID_LOG_DEBUG, "PROXIMITY", msg)

int main(int argc, char *argv[]) {
    proximity_params_t config;
    char message[128];

    int fd = open_sensor(config);
    if (fd < 0)
        return -1;

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

        int proximity = poll_sensor(fd, precision);

        if ((proximity < config.proximity.min)) {
            close_sensor(fd);
            return 0;
        }

        snprintf(message,
                 sizeof(message),
                 "proximity @%2d: %4.2f",
                 precision,
                 (100.0 * (proximity - config.proximity.min))
 			/ config.proximity.range);
        ALOG(message);
    }
}
