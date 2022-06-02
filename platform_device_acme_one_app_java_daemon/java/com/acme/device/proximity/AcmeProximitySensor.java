package com.acme.device.proximity;

import java.io.IOException;
import java.lang.AutoCloseable;
import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.util.concurrent.atomic.AtomicLong;
import java.util.Map;
import java.util.HashMap;
import javax.annotation.Nonnull;
import javax.annotation.concurrent.GuardedBy;

public class AcmeProximitySensor implements AutoCloseable {
    private static final class SensorCleaner
        extends PhantomReference<AcmeProximitySensor> {
        private final AtomicLong peerRef;

        SensorCleaner(AtomicLong peerRef, AcmeProximitySensor sensor) {
            super(sensor, REF_QUEUE);
            this.peerRef = peerRef;
        }

        void cleanup() { AcmeProximitySensor.cleanup(peerRef); }
    }

    static final ReferenceQueue<AcmeProximitySensor> REF_QUEUE
        = new ReferenceQueue<>();

    @Nonnull
    public static ScheduledTask getScheduledTask() {
        return new ScheduledTask(AcmeProximitySensor::cleanup, 100);
    }

    private static void cleanup() {
        Reference<? extends AcmeProximitySensor> ref;
        while ((ref = REF_QUEUE.poll()) != null) {
            ((SensorCleaner)ref).cleanup();
        }
    }

    static final Map<AtomicLong, Reference<?>> CLEANERS = new HashMap<>();

    static {
        try {
            System.load("/vendor/lib/libacmeproximityjni.so");
        } catch (Throwable e) {
            System.out.println("ERROR: Failed to load JNI library: " + e.getMessage());
            System.exit(-1);
        }
     }

    @Nonnull
    public static AcmeProximitySensor getSensor() {
        synchronized (CLEANERS) {
            final AtomicLong peerRef = new AtomicLong(open());
            final AcmeProximitySensor sensor = new AcmeProximitySensor(peerRef);
            CLEANERS.put(peerRef, new SensorCleaner(peerRef, sensor));
            return sensor;
        }
    }

    static void cleanup(AtomicLong peerRef) {
        final long peer = peerRef.getAndSet(0);
        if (peer == 0) { return; }

        synchronized (CLEANERS) {
            CLEANERS.remove(peerRef);
            close(peer);
        }
    }

    private final AtomicLong peerRef;

    private AcmeProximitySensor(AtomicLong peerRef) { this.peerRef = peerRef; }

    public int poll(int precision) throws IOException {
        synchronized (CLEANERS) {
            final long peer = peerRef.get();
            if (peer == 0) { throw new IOException("Device not open"); }
            return poll(peer, precision);
        }
    }

    @Override
    public void close() { cleanup(peerRef); }

    @GuardedBy("CLEANERS")
    private static native void close(long handle);

    @GuardedBy("CLEANERS")
    private static native long open();

    @GuardedBy("CLEANERS")
    private static native int poll(long handle, int precision);
}
