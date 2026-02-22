# LDR Based Weighted Algorithm Solar Tracker with WiFi Integration

### ESP32-WROOM-32 \| Dual-Core FreeRTOS \| Embedded Web Dashboard

------------------------------------------------------------------------

## Abstract

This project presents a single-axis solar tracking system using an
**ESP32-WROOM-32**, four Light Dependent Resistors (LDRs), and
servo-based actuation.

Unlike threshold-based tracking systems, this implementation uses a
**continuous weighted-average control algorithm**, producing smooth
motion, reduced mechanical stress, and improved angular stability.

The system integrates WiFi telemetry and hosts a real-time web dashboard
accessible via:

    http://esp32-solar.local

------------------------------------------------------------------------

# 1. System Architecture

## 1.1 Functional Overview

| Stage          | Description                                     |
|----------------|-------------------------------------------------|
| **Sensing**    | 4 × LDRs arranged at predefined angular offsets |
| **Processing** | ESP32-WROOM-32 running dual-core FreeRTOS       |
| **Actuation**  | Servo motor (0°–180° rotation)                  |
| **Monitoring** | Embedded HTTP server with mDNS                  |

------------------------------------------------------------------------

## 1.2 FreeRTOS Core Allocation

| Core       | Responsibility                                                       |
|------------|----------------------------------------------------------------------|
| **Core 0** | LDR acquisition, weighted computation, servo smoothing, ADC sampling |
| **Core 1** | WiFi SoftAP, HTTP server, mDNS resolution                            |

------------------------------------------------------------------------

# 2. Hardware Implementation

## 2.1 Microcontroller

-   ESP32-WROOM-32  
-   12-bit ADC  
-   Integrated WiFi  
-   PWM servo control

## 2.2 LDR Configuration

Four LDRs positioned at:

-   30°  
-   70°  
-   110°  
-   150°

------------------------------------------------------------------------

## 📷 1. Circuit Diagram

    images/circuit-diagram.png

![Circuit Diagram](images/circuit-diagram.png)

------------------------------------------------------------------------

## 📷 2. Prototype Setup

    images/prototype-setup.jpg

![Prototype Setup](images/prototype-setup.jpg)

------------------------------------------------------------------------

## 📷 2.5 Multimeter Validation

    images/multimeter-validation.jpg

![Multimeter Validation](images/multimeter-validation.jpg)

------------------------------------------------------------------------

# 3. Voltage Measurement Model

## 3.1 Divider Configuration

-   External resistor: **4.5 kΩ**
-   Internal pull-up: **45 kΩ**

Approximate scaling:

$$
\\frac{45k}{4.5k} \\approx 10 \\Rightarrow \\text{Effective Scaling} \\approx 11:1
$$

------------------------------------------------------------------------

## 3.2 Voltage Conversion Formula

$$
V\_{\\text{panel}} = \\left(\\frac{ADC}{4095}\\right) \\times 3.3 \\times 11
$$

Where:

-   *A**D**C* = 12-bit analog reading  
-   3.3V = ESP32 ADC reference  
-   11 = divider scaling factor

------------------------------------------------------------------------

# 4. Weighted Tracking Algorithm

## 4.1 Mathematical Model

Let:

-   *w*<sub>*i*</sub> = normalized light intensity from sensor *i*
-   *θ*<sub>*i*</sub> = physical angle of sensor *i*

Target servo angle:

$$
\\theta = \\frac{\\sum\_{i=1}^{4} w_i \\theta_i}{\\sum\_{i=1}^{4} w_i}
$$

------------------------------------------------------------------------

## 4.2 Servo Smoothing

``` cpp
if(currentServoPos < servoAngle) currentServoPos++;
else if(currentServoPos > servoAngle) currentServoPos--;
```

This prevents oscillatory jitter and mechanical stress.

------------------------------------------------------------------------

# 5. REST API Endpoints

| Endpoint   | Function                      |
|------------|-------------------------------|
| `/data`    | Returns live telemetry (JSON) |
| `/history` | Returns voltage history       |
| `/control` | Manual/Auto override          |

------------------------------------------------------------------------

# 6. Web Dashboard Features

-   Real-time voltage display  
-   Calculated power output  
-   Servo angle monitoring  
-   Raw LDR readings  
-   Manual/Auto override  
-   Voltage history visualization  
-   Weather-based irradiance simulation

------------------------------------------------------------------------

# 7. Experimental Validation

Include the following images in your repository:

| Evidence Type         | File Path                          |
|-----------------------|------------------------------------|
| Circuit Diagram       | `images/circuit-diagram.png`       |
| Prototype Setup       | `images/prototype-setup.jpg`       |
| Multimeter Validation | `images/multimeter-validation.jpg` |
| Dashboard UI          | `images/dashboard-ui.png`          |

------------------------------------------------------------------------

# 8. Limitations

-   Single-axis tracking only  
-   LDR sensitivity to diffuse ambient light  
-   No direct current sensing (power estimated)  
-   No seasonal declination compensation

------------------------------------------------------------------------

# 9. Future Work

-   [ ] Dual-axis implementation  
-   [ ] INA219 current sensing  
-   [ ] MPPT integration  
-   [ ] SD card logging  
-   [ ] MQTT telemetry  
-   [ ] Cloud dashboard integration

------------------------------------------------------------------------

# Key Engineering Concepts Demonstrated

-   Analog signal conditioning  
-   Voltage divider scaling  
-   ADC calibration  
-   PWM servo control  
-   Closed-loop control systems  
-   Weighted averaging algorithms  
-   FreeRTOS task scheduling  
-   Embedded web servers  
-   mDNS networking

------------------------------------------------------------------------

**Author:**  
Akashneel
ESP32-WROOM-32  
2025