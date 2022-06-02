package com.acme.one.permssampleclient

import android.app.Activity
import android.content.*
import android.os.Bundle
import android.os.Handler
import android.os.Message
import android.os.Messenger
import android.util.Log
import android.widget.Button
import android.widget.TextView

class MainActivity : Activity(), Handler.Callback {
    companion object {
        private const val TAG = "MainActivity"
        private const val ACME_MIN_TGT_RXR_PKG = "com.acme.one.permssamplehost"
        private const val ACME_MIN_TGT_RXR =
            ACME_MIN_TGT_RXR_PKG + ".MinTargetReceiver"
        private const val MESSAGE_MIN_TGT_SDK = 1000
        private const val ACTION_GET = "com.acme.one.action.GET"
        private const val ACTION_NOTIFY = "com.acme.one.action.NOTIFY"
        private const val ACTION_NOTIFY_RESP = "com.acme.one.action.NOTIFY_RESP"
        private const val EXTRA_MESSENGER = "com.acme.one.extra.EXTRA_MESSENGER"
        private const val EXTRA_MIN_TGT_SDK = "com.acme.one.extra.EXTRA_MIN_TGT_SDK"
    }

    private lateinit var statusText: TextView
    private lateinit var getButton: Button
    private lateinit var notifyButton: Button
    private val handler = Handler(this)
    private val messenger = Messenger(handler)
    private var active = false

    private val respRxr = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            when (intent.action) {
                ACTION_NOTIFY_RESP -> {
                    val minTgtSdk = intent.getIntExtra(EXTRA_MIN_TGT_SDK, -1)
                    if (minTgtSdk != -1) {
                        handler.obtainMessage(MESSAGE_MIN_TGT_SDK, minTgtSdk, 0)
                            .sendToTarget()
                    } else {
                        Log.w(TAG, "[onReceive] no min target SDK provided")
                    }
                }
            }
        }

    }

    private fun createReceiverIntent(action: String): Intent {
        val intent = Intent(action)
        intent.component = ComponentName(ACME_MIN_TGT_RXR_PKG, ACME_MIN_TGT_RXR)
        return intent
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        statusText = findViewById(R.id.status_text)
        getButton = findViewById(R.id.btn_get)
        notifyButton = findViewById(R.id.btn_notify)

        getButton.setOnClickListener {
            val getIntent = createReceiverIntent(ACTION_GET)
            getIntent.putExtra(EXTRA_MESSENGER, messenger)
            statusText.setText(R.string.using_get)
            Log.d(TAG, "[GET] requesting min target SDK")
            sendBroadcast(getIntent)
        }

        notifyButton.setOnClickListener {
            val notifyIntent = createReceiverIntent(ACTION_NOTIFY)
            statusText.setText(R.string.using_notify)
            Log.d(TAG, "[NOTIFY] requesting min target SDK")
            sendBroadcast(notifyIntent)
        }

    }

    override fun onResume() {
        super.onResume()
        statusText.setText(R.string.startup_text)
        registerReceiver(respRxr, IntentFilter(ACTION_NOTIFY_RESP))
        active = true
    }

    override fun onPause() {
        active = false
        unregisterReceiver(respRxr)
        super.onPause()
    }

    override fun handleMessage(msg: Message): Boolean {
        if (!active) {
            Log.w(TAG, "[handleMessage] not active, discard message: $msg")
            return true
        }

        when (msg.what) {
            MESSAGE_MIN_TGT_SDK -> {
                Log.d(TAG, "[handleMessage] received min target SDK: ${msg.arg1}")
                statusText.text = getString(R.string.min_tgt_sdk_fmt, msg.arg1)
            }

            else -> {
                Log.e(TAG, "[handleMessage] unknown message type: ${msg.what}")
            }
        }

        return true
    }
}
