# QuAC ESP32 Control System

Embedded control software and electronics for **QuAC (Quick Aorta Compressor)**, a motorised external-compression prototype developed as part of **TMM4121 – Product Development** at NTNU.

> **Educational prototype:** QuAC was developed as a student product-development project. It is not a certified medical device and is not intended for clinical use.

QuAC was developed by a thirteen-person Mechanical Engineering student team. My responsibilities were within **mechatronics and programming**, shared with one other team member, and included ESP32 implementation, ESP-NOW communication, motor-control logic, electronics integration, testing, and troubleshooting.

This repository documents the **software and electronics** portion of the project. The mechanical design, CAD development, manufacturing, structural analysis, and complete product-development process are presented separately in the project case study on my portfolio website.

<p align="center">
  <img src="images/final-prototype.jpg" alt="Final QuAC prototype" width="650">
  <br>
  <em>Final QuAC prototype.</em>
</p>

---

## Overview

QuAC uses a motor-driven linear actuator to move a compression arm. The mechanism is controlled through a wireless handheld remote, allowing the operator to control the actuator without a wired connection to the main unit.

The embedded system uses two ESP32 development boards:

- **Remote controller** — reads two push buttons and transmits movement commands.
- **Actuator controller** — receives movement commands and controls a 12 V DC gearmotor through an L298N motor driver.

The two ESP32 boards communicate directly using **ESP-NOW**.

### Key Features

- Wireless communication between two ESP32 boards
- Direct ESP-NOW command transmission
- Separate remote-controller and actuator-controller firmware
- Bidirectional DC motor control
- Upward, downward, and stop commands
- Battery-powered remote and actuator units
- Electronics integrated into the completed prototype
- Full-system testing under mechanical load

### Control Flow

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

---

## Electronics

<p align="center">
  <img src="images/electronics-prototype.jpg" alt="QuAC electronics prototype" width="650">
  <br>
  <em>Assembled electronics used for wireless communication and actuator control.</em>
</p>

| Component | Function |
|---|---|
| ESP32, remote unit | Reads button inputs and transmits commands using ESP-NOW |
| ESP32, actuator unit | Receives commands and controls the motor driver |
| Two push buttons | Command upward and downward actuator movement |
| L298N dual H-bridge | Provides bidirectional DC motor control |
| 12 V DC gearmotor | Drives the leadscrew-based linear actuator |
| Battery pack | Powers the motor and motor driver |
| Power bank | Powers the ESP32 control electronics |

---

## Remote Controller

The handheld remote contains an ESP32, two push buttons, and a portable power source.

The ESP32 reads the button states and sends one of three commands:

```text
UP
DOWN
STOP
```

<p align="center">
  <img src="documentation/remote-wiring-diagram.svg" alt="Remote-controller wiring diagram" width="750">
  <br>
  <em>Wiring diagram for the ESP32 remote controller.</em>
</p>

The remote-controller firmware is responsible for:

- Initialising ESP-NOW
- Registering the actuator ESP32 as a communication peer
- Reading the upward and downward buttons
- Encoding the selected command
- Transmitting the command
- Sending a stop command when neither button is pressed

<p align="center">
  <img src="documentation/remote-activity-diagram.svg" alt="Remote-controller activity diagram" width="700">
  <br>
  <em>Remote-controller logic for reading user input and transmitting commands.</em>
</p>

---

## Actuator Controller

The actuator unit contains an ESP32, an L298N motor driver, a 12 V DC gearmotor, and a battery supply.

The actuator ESP32 receives the wireless command and sets the motor-driver inputs accordingly.

<p align="center">
  <img src="documentation/actuator-wiring-diagram.svg" alt="Actuator-controller wiring diagram" width="750">
  <br>
  <em>Wiring diagram for the ESP32 actuator controller, L298N, and DC motor.</em>
</p>

| Command | IN1 | IN2 | Result |
|---|---:|---:|---|
| Move down | HIGH | LOW | Motor rotates forward |
| Move up | LOW | HIGH | Motor rotates in reverse |
| Stop | LOW | LOW | Motor stops |

The physical movement direction depends on the motor wiring and can be reversed in software if required.

The actuator-controller firmware is responsible for:

- Initialising ESP-NOW as a receiver
- Receiving movement commands
- Decoding the received command
- Setting the L298N direction inputs
- Driving the motor upward or downward
- Stopping the motor when no movement command is active

<p align="center">
  <img src="documentation/actuator-activity-diagram.svg" alt="Actuator-controller activity diagram" width="700">
  <br>
  <em>Actuator-controller logic for receiving commands and controlling motor direction.</em>
</p>

---

## Wireless Communication

The two ESP32 boards communicate using **ESP-NOW**, a direct wireless communication protocol supported by the ESP32.

ESP-NOW was selected because it allows short commands to be transmitted directly between two known devices without requiring a router or external Wi-Fi network.

The communication system was developed incrementally:

1. Test values were transmitted between the two ESP32 boards.
2. Received values were verified through serial output.
3. Button inputs were connected to the transmitted data.
4. Received commands were connected to the motor-control logic.
5. The complete wireless system was tested with the actuator.

```text
Remote input
    ↓
Command encoded
    ↓
ESP-NOW transmission
    ↓
Command received
    ↓
Motor direction selected
    ↓
Actuator movement
```

---

## Motor Control

The actuator is driven by a 12 V DC gearmotor.

The motor rotates a leadscrew through a gear transmission. Rotation of the leadscrew moves a nut along the screw, converting rotational motion into linear movement of the compression arm.

The L298N motor driver allows the actuator ESP32 to:

- Drive the motor forward
- Drive the motor in reverse
- Stop the motor

The GitHub repository focuses on the electronic and software control of the mechanism. Detailed mechanical design and manufacturing are documented in the portfolio case study.

---

## Software Structure

The repository contains two separate firmware programs because the ESP32 boards perform different roles.

### Remote-Controller Firmware

```text
software/remote-controller/remote-controller.ino
```

Responsibilities:

- Read button inputs
- Determine the active command
- Send commands through ESP-NOW
- Send a stop command when no button is active

### Actuator-Controller Firmware

```text
software/actuator-controller/actuator-controller.ino
```

Responsibilities:

- Receive ESP-NOW messages
- Decode movement commands
- Control the L298N direction inputs
- Drive or stop the DC motor

---

## Repository Structure

```text
quac-esp32-control/
├── documentation/
│   ├── actuator-activity-diagram.svg
│   ├── actuator-wiring-diagram.svg
│   ├── remote-activity-diagram.svg
│   └── remote-wiring-diagram.svg
├── images/
│   ├── electronics-prototype.jpg
│   └── final-prototype.jpg
├── software/
│   ├── actuator-controller/
│   │   └── actuator-controller.ino
│   └── remote-controller/
│       └── remote-controller.ino
└── README.md
```

- `documentation/` contains the activity and wiring diagrams for both ESP32 units.
- `images/` contains photographs of the electronics and completed prototype.
- `software/remote-controller/` contains the firmware for the handheld remote.
- `software/actuator-controller/` contains the firmware for the motor-control unit.

---

## Setup

### Requirements

- Two ESP32 development boards
- Arduino IDE or another compatible ESP32 development environment
- ESP32 Arduino board package
- ESP-NOW support included with the ESP32 framework

### 1. Configure the Receiver Address

The remote controller must contain the MAC address of the actuator ESP32.

Replace the placeholder address in the remote-controller firmware:

```cpp
uint8_t receiverAddress[] = {
    0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX
};
```

### 2. Upload the Actuator Firmware

Open:

```text
software/actuator-controller/actuator-controller.ino
```

Select the correct ESP32 board and serial port, then compile and upload the firmware.

### 3. Upload the Remote Firmware

Open:

```text
software/remote-controller/remote-controller.ino
```

Select the second ESP32 board and serial port, then compile and upload the firmware.

### 4. Test the System

Before connecting the motor to a loaded mechanism:

1. Confirm that the motor remains stopped when neither button is pressed.
2. Test the upward command.
3. Test the downward command.
4. Confirm that the motor direction matches the intended actuator movement.
5. Confirm that the motor stops when the button is released.
6. Test the actuator without mechanical load before applying full load.

---

## Testing

The electronics were developed and tested incrementally before integration into the completed prototype.

Testing included:

- Push-button input
- ESP-NOW communication
- Transmission of test values
- Motor direction
- Motor-driver response
- Wireless remote operation
- Integration with the linear actuator
- Full-system testing under mechanical load

The L298N motor driver produced significant heat during testing. It was therefore mounted on 3D-printed standoffs to improve airflow and reduce direct contact with the baseplate.

A potentiometer was originally included for variable speed control. It was removed after failing late in the development process and was not required for the prototype's basic operation.

A force sensor was also considered but not implemented because of sensor capacity, packaging, wiring, and integration constraints within the project timeframe.

During full-system testing, the complete prototype produced a measured compression force of **45.8 kg**, exceeding the engineering target of 40 kg.

> This was a prototype load test and not a clinical validation.

---

## Known Limitations

- No automatic fail-safe stops the motor if ESP-NOW communication is lost.
- No limit switches prevent excessive actuator travel.
- The L298N motor driver produces significant heat under sustained load.
- The ESP32 electronics and motor use separate power sources.
- The system uses open-loop direction control without force feedback.
- The prototype does not include a dedicated hardware emergency stop.

---

## Future Improvements

- Add upper and lower limit switches
- Implement communication-loss fail-safe behaviour
- Add a hardware emergency stop
- Replace the L298N with a more efficient motor driver
- Use a regulated power system for the motor and control electronics
- Add calibrated force sensing
- Implement closed-loop force control

---

## Contributions

The complete QuAC prototype was developed by a thirteen-person Mechanical Engineering student team at NTNU.

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
