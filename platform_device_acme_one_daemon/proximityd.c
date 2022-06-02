#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <android/log.h>

#define SERIAL_DEVICE_FILE      "/dev/ttyUSB0"
#define SAMPLE_BUF_SIZE         1024

#define ALOG(msg) __android_log_write(ANDROID_LOG_DEBUG, "PROXIMITYD", msg)

int main(int argc, char* argv[]) {
    int result = -1;
    int fd = -1;
    char buf[SAMPLE_BUF_SIZE];
    ssize_t count = 0;

//  LJS: Debug
for (int i = 0; i < 10; i++) {
    ALOG("Sleeping for a bit...");
    sleep(5);
    ALOG("Rise and shine!");
}
return 0;

    ALOG("Opening device file\n");
    fd = open(SERIAL_DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        ALOG("Failed to open device file");
        goto _errExit;
    }

    ALOG("Reading from device file\n");
    count = read(fd, buf, SAMPLE_BUF_SIZE);
    if (count < 0) {
        ALOG("Failed to read from device file");
    }

    ALOG("Writing to device file\n");
    buf[0] = '1';
    buf[1] = '2';
    buf[2] = '3';
    buf[3] = '4';
    count = write(fd, buf, 4);
    if (count < 0) {
        ALOG("Failed to write to device file");
    } 

_errExit:
    if (fd >= 0) {
        close(fd);
    }

    ALOG("Done");
    return result;
}
