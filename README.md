# ESP32 Wireless Linear Actuator Control

Embedded software and electronics for the wireless control of a custom, DC motor-driven linear actuator developed as part of **TMM4121 – Product Development** at NTNU.

The system uses two ESP32 development boards communicating through **ESP-NOW**. A handheld remote reads two push buttons and transmits movement commands to a second ESP32, which controls a 12 V DC gearmotor through an L298N motor driver.

> **Educational prototype:** This repository documents a student engineering prototype. The complete system is not a certified medical device and is not intended for clinical use.

My responsibilities were within **mechatronics and programming**, shared with one other team member, and included ESP32 implementation, ESP-NOW communication, motor-control logic, electronics integration, testing, and troubleshooting.

This repository covers only the **embedded software, electronics, wireless communication, and actuator-control system**. The wider product context, mechanical design, CAD, manufacturing, structural analysis, and product-development process are documented separately in the project case study on my portfolio website.

---

## Overview

The system provides wireless, bidirectional control of a custom leadscrew-based linear actuator.

Two ESP32 boards perform separate roles:

- **Remote controller** — reads the operator's button inputs and transmits movement commands.
- **Actuator controller** — receives the commands and controls the DC motor through an L298N H-bridge.

```text
Push buttons
    ↓
Remote ESP32
    ↓  ESP-NOW
Actuator ESP32
    ↓
L298N motor driver
    ↓
12 V DC gearmotor
    ↓
Linear actuator movement
```

### Key Features

- Direct wireless communication using ESP-NOW
- Separate transmitter and receiver firmware
- Bidirectional DC motor control
- Upward, downward, and stop commands
- Battery-powered remote and actuator units
- Integration with a custom leadscrew actuator
- Full-system testing under mechanical load

---

## Hardware and Wiring

<p align="center">
  <img src="images/electronics-prototype.jpg" alt="ESP32 and motor-control electronics during prototyping" width="600">
  <br>
  <em>Electronics prototype used during communication and motor-control development.</em>
</p>

| Component | Function |
|---|---|
| ESP32, remote unit | Reads button inputs and transmits movement commands |
| ESP32, actuator unit | Receives commands and controls the motor driver |
| Two push buttons | Command upward and downward actuator movement |
| L298N dual H-bridge | Provides bidirectional DC motor control |
| 12 V DC gearmotor | Drives the leadscrew-based linear actuator |
| Battery pack | Powers the motor and motor driver |
| Power bank | Powers the ESP32 control electronics |

### ESP32 Pin Configuration

#### Remote Controller

| Function | GPIO | Configuration |
|---|---:|---|
| Down button | 16 | `INPUT_PULLUP` |
| Up button | 17 | `INPUT_PULLUP` |

The buttons use the ESP32's internal pull-up resistors and are active-low:

```text
LOW  = button pressed
HIGH = button released
```

#### Actuator Controller

| Function | GPIO | Connection |
|---|---:|---|
| Motor enable | 25 | L298N ENA |
| Motor direction input 1 | 32 | L298N IN1 |
| Motor direction input 2 | 33 | L298N IN2 |

### Remote-Controller Wiring

<p align="center">
  <img src="documentation/remote-wiring-diagram.png" alt="Remote-controller wiring diagram" width="520">
  <br>
  <em>Wiring diagram for the ESP32 remote controller.</em>
</p>

### Actuator-Controller Wiring

<p align="center">
  <img src="documentation/actuator-wiring-diagram.png" alt="Actuator-controller wiring diagram" width="760">
  <br>
  <em>Wiring diagram for the ESP32 actuator controller, L298N motor driver, and DC motor.</em>
</p>

---

## Firmware

The repository contains one firmware program for each ESP32.

### Remote Controller

```text
software/remote-controller.ino
```

The remote-controller firmware:

- Initialises ESP-NOW
- Registers the actuator ESP32 as a communication peer
- Reads the up and down buttons
- Stores the button states in a shared message structure
- Transmits the control message every 50 ms
- Reports configuration and queueing errors through the serial monitor

The transmitted message contains:

```cpp
typedef struct {
    uint8_t downPressed;
    uint8_t upPressed;
    uint32_t sequence;
} ControlMessage;
```

<p align="center">
  <img src="documentation/remote-activity-diagram.svg" alt="Remote-controller activity diagram" width="500">
  <br>
  <em>Remote-controller logic for reading button inputs and transmitting movement commands.</em>
</p>

### Actuator Controller

```text
software/actuator-controller.ino
```

The actuator-controller firmware:

- Initialises ESP-NOW as a receiver
- Receives the button states
- Decodes the movement command
- Sets the L298N direction inputs
- Drives the motor in the commanded direction
- Stops the motor when both buttons are released, both are pressed, or
  communication is lost for 750 ms

| Input state | IN1 | IN2 | Result |
|---|---:|---:|---|
| Down button pressed | HIGH | LOW | Motor rotates forward |
| Up button pressed | LOW | HIGH | Motor rotates in reverse |
| Both buttons released | LOW | LOW | Motor stops |

<p align="center">
  <img src="documentation/actuator-activity-diagram.svg" alt="Actuator-controller activity diagram" width="720">
  <br>
  <em>Actuator-controller logic for receiving commands and controlling motor direction.</em>
</p>

---

## Wireless Communication

The two ESP32 boards communicate using **ESP-NOW**, a direct wireless protocol supported by the ESP32.

ESP-NOW was selected because it allows short control messages to be exchanged directly between two known devices without requiring a router or external Wi-Fi network.

The actuator ESP32 is registered as a peer using its MAC address. The public repository uses a placeholder value:

```cpp
uint8_t receiverAddress[] = {
    0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX
};
```

The placeholder must be replaced with the MAC address of the actuator ESP32 before uploading the remote-controller firmware.

---

## Testing and Results

The electronics were developed incrementally before integration into the complete prototype.

Testing covered:

- Digital button inputs
- ESP-NOW message transmission
- Sender and receiver pairing
- Motor direction control
- Motor-driver response
- Integration with the linear actuator
- Operation under mechanical load

The L298N motor driver produced significant heat during testing. It was therefore mounted on 3D-printed standoffs to improve airflow and reduce direct contact with the baseplate.

During a static load test, the actuator system produced a measured compression output equivalent to **45.8 kgf**, exceeding the 40 kgf engineering target.

> This was a prototype load test and not a clinical validation.

---

## Limitations and Next Steps

- **Communication loss:** The receiver stops the motor when no valid command has
  been received for 750 ms. A production design would also require a
  hardware-level safety circuit and a verified communication-loss test.
- **Actuator travel:** No upper or lower limit switches prevent mechanical over-travel. Hardware limits should be added.
- **Motor driver:** The L298N produced significant heat under sustained load. A more efficient motor driver would improve reliability and power efficiency.
- **Control feedback:** The system operates without force feedback. A calibrated force sensor could enable closed-loop force control.
- **Emergency stop:** The prototype does not include a dedicated hardware emergency-stop circuit.
- **Firmware robustness:** Conflicting button commands are rejected. Hardware
  button debouncing and stronger message validation should still be added.

---

## Repository Structure

```text
esp32-wireless-linear-actuator/
├── documentation/
│   ├── actuator-activity-diagram.svg
│   ├── actuator-wiring-diagram.png
│   ├── remote-activity-diagram.svg
│   └── remote-wiring-diagram.png
├── images/
│   └── electronics-prototype.jpg
├── software/
│   ├── actuator-controller.ino
│   └── remote-controller.ino
├── .gitignore
└── README.md
```

- `documentation/` contains the activity and wiring diagrams.
- `images/` contains a photograph from electronics prototyping.
- `software/remote-controller.ino` contains the transmitter firmware.
- `software/actuator-controller.ino` contains the receiver and motor-control firmware.

---

## Setup

### Requirements

- Two ESP32 development boards
- Arduino IDE or another compatible ESP32 development environment
- ESP32 Arduino board package
- ESP-NOW support included with the ESP32 framework
- L298N motor driver
- Compatible DC motor and power supply

### 1. Upload the Actuator Firmware

Open:

```text
software/actuator-controller.ino
```

Select the correct ESP32 board and serial port, then compile and upload the firmware.

### 2. Configure the Receiver Address

Find the MAC address of the actuator ESP32 and replace the placeholder in the remote-controller firmware:

```cpp
uint8_t receiverAddress[] = {
    0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX
};
```

### 3. Upload the Remote Firmware

Open:

```text
software/remote-controller.ino
```

Select the second ESP32 board and serial port, then compile and upload the firmware.

### 4. Perform an Initial Test

Before applying mechanical load:

1. Confirm that the motor remains stopped when both buttons are released.
2. Test the downward command.
3. Test the upward command.
4. Confirm that the movement direction matches the intended actuator direction.
5. Confirm that releasing the button stops the motor.
6. Test the actuator without load before performing a loaded test.

---

## Contributions

The complete prototype was developed by a thirteen-person Mechanical Engineering student team at NTNU.

The mechatronics sub-team consisted of **Mohamed Elwalid Fadul** and **Hardik Deshpande**. The sub-team was responsible for the electronic components, firmware, testing, and integration of the mechatronic system into the physical prototype.

My main contributions included:

- ESP32 implementation
- ESP-NOW communication
- Remote-controller firmware
- Motor-control logic
- Electronics wiring
- Component testing
- Integration with the linear actuator
- Full-system troubleshooting
