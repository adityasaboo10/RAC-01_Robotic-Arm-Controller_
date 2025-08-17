# RAC-01_Robotic-Arm-Controller_

## Description
This project was developed as a solution to the problem statement for the Arduino SSCS Competition 2025.  
**The RAC (Robotic Arm Controller)** is a custom-built robotic arm designed to assist individuals with limited mobility due to physical impairments or limb loss. It supports multiple control modes (Joystick, Press-and-Play, and Bluetooth), offering intuitive and flexible operation.

---

## Table of Contents
1. [Hardware Materials](#hardware-materials)
2. [Setup](#setup)
3. [Versions](#versions)
4. [Usage](#usage)
5. [Suggestions](#suggestions)
6. [Gallery](#gallery)

---

## Hardware Materials

### Electronics
- **Arduino Uno** – Used for Joystick, Press-and-Play, and Bluetooth control modes
- **HC-05** – Bluetooth module
- **ESP32 (38-pin)** – Used for Bluetooth version (see compatibility notes below)
- **MG90 Servo Motors** (x4) – Actuate base, arm, elbow, and claw
- **18650 Batteries** – Power source for servos
- **MINI560** – Buck Converter
- **HW-107 Charging Module** – Charges one 18650 battery at a time
- **Other** – Jumper wires, breadboard, perfboard, 18650 2S holder, soldering iron, solder wire

### Mechanical / Printed Parts
- **3D Printed Components** – Base, arm, claw, cap
- **Fasteners** – Screws, bolts, rubber bands, double-sided tape, etc.

### Materials
- **PLA Filament** – Used for all 3D printed parts

---

## Setup
**EASYEDA circuit**-[`RAC_schematic_pcb.epro`](RAC_schematic_pcb.epro)

---

## Versions

### 🔹 Arduino Versions (Stable)
These versions are **fully tested** and support all features of RAC, including Joystick, Press-and-Play, and Bluetooth control.

| Version | Description | File |
|---------|-------------|------|
| 🕹️ Joystick Only | Basic joystick control for the robotic arm. | [`Joystick_testcode.ino`](Joystick_testcode.ino) |
| 🕹️+▶️ Press and Play | Supports joystick control with a press-and-play mode to repeat motions. | [`Joystick controlled with press and play feature.ino`](Joystick%20controlled%20with%20press%20and%20play%20feature.ino) |
| 🔄 Full Mode Switching | Mode-switching via clicks (elbow, arm, claw, base, reset) | [`Controlling_via-Joystick.ino`](Controlling_via-Joystick.ino) |
| 📱 Bluetooth (HC-05) | Bluetooth control using Dabble App via HC-05 on Arduino. | [`Bluetooth_controlled_arduino.ino`](Bluetooth_controlled_arduino.ino) |
| 📶 HC-05 Test Code | Checks serial communication with HC-05. | [`testcode_for_HC05.ino`](testcode_for_HC05.ino) |

---

### 🔸 ESP32 Versions (⚠️ Experimental)
These are meant for Bluetooth-only control. **Servos do not work yet** due to ESP32’s 3.3V PWM signal being too weak for MG90 servos.

| Version | Description | File |
|---------|-------------|------|
| 📱 Bluetooth (ESP32) | Full control using Dabble App and ESP32. **Servos unresponsive** due to logic level mismatch. | [`Bluetooth_controlled_via_esp.ino`](Bluetooth_controlled_via_esp.ino) |
| 🎮 Gamepad Test | Gamepad test using Dabble’s directional pad. | [`Gamepad_testcode_for_esp32.ino`](Gamepad_testcode_for_esp32.ino) |

> ⚠️ **Note:** MG90S servos do not detect ESP32's 3.3V PWM signal. Consider using a logic-level shifter or a servo driver like PCA9685 to resolve this.


---

## Usage
- Choose the control mode via button interface (clicks/presses)
- Power servos via 18650 pack
- Upload the appropriate sketch based on the board (Arduino or ESP32)

---

## Suggestions & Improvements

- ✅ **Fix PWM voltage issue**: Use transistor or level shifter to make ESP32 compatible with MG90 servos
- 🔄 **Add auto-calibration** for reset position
- 🧠 **Integrate feedback** using rotary encoders or limit switches
- 💡 **Improve power management** – current sensing or buck converter with display
- 📱 **Android app** for Bluetooth control (currently using Dabble)

---

## 🎥 Gallery
### 🎥 Project Demo

*My Final Submission Video* →  [![Watch on YouTube](https://img.shields.io/badge/Watch%20on-YouTube-red?logo=youtube&logoColor=white)](https://youtu.be/XiNYWbxc35A)


---

