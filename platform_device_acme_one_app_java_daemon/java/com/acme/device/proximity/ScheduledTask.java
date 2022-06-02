package com.acme.device.proximity;

public class ScheduledTask extends Thread {
    private long checkInterval;
    private Runnable checkTask;
    private boolean exit = false;

    public ScheduledTask(Runnable task, long interval) {
        checkTask = task;
        checkInterval = interval;
        setDaemon(true);
        start();
    }

    @Override
    public void run() {
        while (!exit) {
            try {
                sleep(checkInterval);

                if (exit) {
                    continue;
                }

                checkTask.run();
            } catch (InterruptedException e) {
                //  Nothing to do, loop checks exit condition
            }
        }
    }

    public void exit() {
        exit = true;
        interrupt();

        try {
            join();
        } catch (InterruptedException e) {
            System.out.println("Interrupted while joining exited thread.");
        }
    }
}
