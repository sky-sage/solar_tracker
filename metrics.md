# PART II — Performance Evaluation & Engineering Analysis

---

## Performance Comparison

The weighted tracking algorithm was evaluated against a static fixed-panel configuration.

| Parameter                  | Static Panel | Weighted Tracker |
| -------------------------- | ------------ | ---------------- |
| Morning Irradiance Capture | Low          | Improved         |
| Midday Stability           | Moderate     | High             |
| Late-Afternoon Efficiency  | Low          | Improved         |
| Servo Oscillation          | None         | Minimal          |
| Angular Accuracy           | Fixed        | Adaptive         |
| Energy Exposure Duration   | Limited      | Extended         |

The weighted algorithm demonstrated improved exposure duration and smoother mechanical behavior relative to static mounting.

---

## 📷 System Flowchart

<img src="images/flowchart.png" width="500"/>

---

## Engineering Observations

1. Weighted averaging eliminates threshold-induced oscillation.
2. Servo smoothing significantly reduces mechanical stress.
3. Dual-core separation enhances determinism.
4. mDNS simplifies deployment without IP discovery.
5. LDR-based sensing is cost-effective but sensitive to diffuse light.

---

## Limitations

* Single-axis tracking only
* No seasonal declination compensation
* Power estimation assumes constant current
* No integrated MPPT

---

## Future Work

* Dual-axis tracking
* INA219 current sensing
* MPPT integration
* SD card logging
* MQTT telemetry
* Cloud-based monitoring

---

## Key Technical Concepts Demonstrated

* Analog signal conditioning
* Voltage divider scaling
* ADC calibration
* PWM servo control
* Closed-loop control systems
* Weighted average computation
* Dual-core FreeRTOS scheduling
* Embedded HTTP server implementation
* mDNS networking

---

## Platform & License

| Attribute            | Specification                   |
| -------------------- | ------------------------------- |
| Microcontroller      | ESP32-WROOM-32                  |
| Development Platform | PlatformIO                      |
| RTOS                 | FreeRTOS (Dual-Core)            |
| License              | GNU General Public License v2.0 |

---
