package com.acme.device.proximity;

import android.util.Log;
import java.io.IOException;
import java.lang.Thread;

public class AcmeJavaDaemon {
    private static final String TAG = "AcmeJavaDaemon";
    private static final long SLEEP_TIME_MS = 60000;

    public static void main(String[] args) {
        System.out.println("Started");

        AcmeJavaDaemon daemon = new AcmeJavaDaemon();
        ScheduledTask task = AcmeProximitySensor.getScheduledTask();
        daemon.monitorSensor();
        task.exit();

        System.out.println("Exiting");
        Log.i(TAG, "Exiting");
    }

    private void monitorSensor() {
        AcmeProximitySensor sensor = AcmeProximitySensor.getSensor();
        if (sensor == null) {
            System.out.println("No sensor available!");
            Log.e(TAG, "No sensor available!");
            return;
        }

        Thread currentThread = Thread.currentThread();
        int i = 0;
        int precision = 40;
        while (true) {
            try {
                currentThread.sleep(SLEEP_TIME_MS);
            } catch (InterruptedException e) {
                Log.w(TAG, "Monitor loop interrupted");
                break;
            }

            i++;
            if (i < 10) {
                precision = 40;
            } else {
                i = 0;
                precision = 80;
            }

            try {
            int proximity = sensor.poll(precision);
                String msg =
                    String.format(
                        "proximity @%2d: %d",
                        precision,
                        proximity);
                Log.i(TAG, msg);
            } catch (IOException e) {
                Log.e(TAG, "Error reading from proximity sensor: " + e.getMessage());
                break;
            }
        }

        sensor.close();
    }
}
