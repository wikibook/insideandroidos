package com.acme.one.aproximityclient

import android.annotation.SuppressLint
import android.app.Activity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast

import vendor.acme.one.aproximity.V1_0.IAproximity
import vendor.acme.one.aproximity.V1_0.ProximitySummary
import java.lang.NumberFormatException

class MainActivity : Activity() {
    companion object {
        const val TAG = "MainActivity"
    }
    private lateinit var proxDetails: TextView
    private lateinit var proxSummary: TextView
    private lateinit var proxPrecision: EditText
    private lateinit var proxValue: TextView
    private lateinit var pollButton: Button
    private lateinit var proxHidl: IAproximity

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        setupHalAccess()
        setupViews()
    }

    override fun onResume() {
        super.onResume()

        updateDetails()
    }

    @SuppressLint("SetTextI18n")
    private fun setupViews() {
        proxDetails = findViewById(R.id.prox_details)
        proxSummary = findViewById(R.id.prox_summary)
        proxValue = findViewById(R.id.prox_data)
        proxPrecision = findViewById(R.id.precision)
        pollButton = findViewById(R.id.get_proximity)
        pollButton.setOnClickListener {
            val precision = try {
                proxPrecision.text.toString().toInt()
            } catch (e: NumberFormatException) {
                null
            }

            precision?.also {
                val proximity: Int = proxHidl.poll(precision)
                Log.d(TAG, "Latest proximity: $proximity")
                proxValue.text = proximity.toString()

                //  Also update summarize details
                val summary = proxHidl.summarize()
                val count = summary.pollCallCount
                val timeStamp = summary.lastPollCalledMs
                proxSummary.text =
                    "Poll called $count times, last at $timeStamp"

            } ?: run {
                Toast.makeText(
                    this,
                    R.string.bad_precision,
                    Toast.LENGTH_LONG
                ).show()
            }
        }
    }

    private fun setupHalAccess() {
        proxHidl = IAproximity.getService(true)
    }

    private fun updateDetails() {
        val details = proxHidl.get_details()
        val precMin = details.precision.min
        val precMax = details.precision.max
        val proxMin = details.proximity.min
        val proxMax = details.proximity.max
        val detailText =
            "Precision ($precMin / $precMax)," +
                    "Proximity ($proxMin / $proxMax)"
        proxDetails.text = detailText
    }
}
