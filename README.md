# QuAC ESP32 Control System

Embedded control software and electronics for **QuAC — Quick Aorta Compressor**, developed as part of **TMM4121 – Product Development** at NTNU.

QuAC was developed by a team of twelve Mechanical Engineering students as an educational prototype intended to automate external aortic compression during postpartum haemorrhage. My responsibilities were within **mechatronics and programming**, including ESP32 implementation, wireless communication, motor control, electronics integration, testing, and system integration.

This repository documents the software and electronics portion of the project. The mechanical design, CAD development, manufacturing, structural analysis, and complete product-development process are documented separately in the project case study on my portfolio website.

> **Educational prototype:** QuAC was developed as a student product-development project. It is not a certified medical device and is not intended for clinical use.

<p align="center">
  <img src="images/final-prototype.jpg" alt="Final QuAC prototype" width="600">
  <br>
  <em>Final QuAC prototype.</em>
</p>

---

## Overview

QuAC is a motorised compression prototype designed to apply external pressure through a custom linear actuator.

The system uses two ESP32 development boards:

- One ESP32 is installed in the wireless remote controller.
- One ESP32 is installed in the main unit and controls the actuator motor.

The two boards communicate wirelessly using **ESP-NOW**. Button inputs from the remote controller are transmitted to the actuator controller, which drives a 12 V DC gearmotor through an L298N motor driver.

The motor rotates a leadscrew through a gear transmission, converting rotational motion into linear movement of the compression arm.

### Key Features

- Wireless remote control using two ESP32 boards
- ESP-NOW communication
- Separate remote-controller and actuator-controller firmware
- Bidirectional DC motor control
- Motor direction controlled through two push buttons
- Custom leadscrew-based linear actuator
- L298N dual H-bridge motor driver
- Portable battery-powered prototype
- Electronics integrated into the complete mechanical system

During final testing, the complete prototype produced a measured compression force of **45.8 kg**, exceeding the project target of 40 kg. This was a prototype performance test and not a clinical validation. PRODUKTUTVIKLING.pdf

---

## System Architecture

<p align="center">
  <img src="documentation/system-architecture.png" alt="QuAC control-system architecture" width="750">
  <br>
  <em>System architecture showing communication between the remote controller and actuator controller.</em>
</p>

The system is divided into two main electronic units.

### Remote Controller

The remote controller contains:

- ESP32 development board
- Two push buttons
- Portable power source

The two buttons command upward and downward movement of the actuator.

The remote ESP32 reads the button states and sends the selected movement command wirelessly to the actuator controller.

### Actuator Controller

The main unit contains:

- ESP32 development board
- L298N motor driver
- 12 V DC gearmotor
- Battery supply
- Custom linear actuator

The actuator ESP32 receives the command and determines the required motor direction.

The L298N motor driver supplies the motor with the current required to move the actuator, while the ESP32 provides the direction and control signals.

---

## Electronics and Wiring

<p align="center">
  <img src="images/integrated-electronics.jpg" alt="Integrated QuAC electronics" width="600">
  <br>
  <em>Electronics integrated into the QuAC prototype.</em>
</p>

| Component | Function |
|---|---|
| ESP32 remote controller | Reads button inputs and transmits commands through ESP-NOW |
| ESP32 actuator controller | Receives commands and controls the motor driver |
| Two push buttons | Command upward and downward actuator movement |
| L298N motor driver | Provides bidirectional control of the DC motor |
| 12 V DC gearmotor | Drives the leadscrew-based actuator |
| Battery pack | Supplies the motor and motor driver |
| Power bank | Supplies the ESP32 control electronics |
| Custom linear actuator | Converts motor rotation into linear motion |

<p align="center">
  <img src="documentation/wiring-diagram.png" alt="QuAC wiring diagram" width="750">
  <br>
  <em>Simplified wiring diagram for the remote controller and actuator-controller electronics.</em>
</p>

The remote controller and main unit are electrically separate.

The remote ESP32 reads the buttons and transmits a movement command. The actuator ESP32 receives the command and controls the L298N input pins.

The motor driver changes the direction of the DC motor according to the received command:

```text
Forward command  -> actuator moves downward
Reverse command  -> actuator moves upward
No command       -> motor stops
```

The report documents that the mechatronics subgroup developed the system incrementally, first testing simple button and LED control, then motor control, and finally wireless communication between the two ESP32 boards. PRODUKTUTVIKLING.pdf

---

## Wireless Communication

The two ESP32 boards communicate using **ESP-NOW**, a direct wireless communication protocol supported by the ESP32.

The development process began by sending test values between the two boards. Once communication was confirmed, the transmitted data was connected to the remote-controller buttons and the actuator motor-control logic.

The communication flow is:

```text
Button input
     |
     v
Remote ESP32
     |
     | ESP-NOW
     v
Actuator ESP32
     |
     v
L298N motor driver
     |
     v
12 V DC motor
     |
     v
Linear actuator movement
```

ESP-NOW was selected because it allows direct communication between ESP32 boards without requiring a separate Wi-Fi network or external controller.

---

## Control Flow

<p align="center">
  <img src="documentation/activity-diagram.png" alt="QuAC software activity diagram" width="750">
  <br>
  <em>Activity diagram showing remote input, wireless transmission, command reception, and motor actuation.</em>
</p>

### Remote-Controller Logic

During each loop, the remote controller:

1. Reads the upward and downward button inputs.
2. Determines whether a valid command is active.
3. Sends the command to the actuator ESP32 using ESP-NOW.
4. Sends a stop command when neither button is pressed.
5. Repeats the loop.

### Actuator-Controller Logic

During each loop, the actuator controller:

1. Checks for a newly received ESP-NOW message.
2. Reads the movement command.
3. Sets the motor-driver direction pins.
4. Activates the motor in the required direction.
5. Stops the motor when no valid movement command is present.
6. Repeats the loop.

---

## Motor and Actuator Control

The system uses a 12 V DC gearmotor because continuous rotation was required to drive the leadscrew mechanism.

The motor drives a pair of gears connected to the leadscrew. Rotation of the leadscrew moves a nut along the screw, producing linear movement of the compression arm.

The mechanical design used the leadscrew because it provided:

- High mechanical advantage
- Controlled linear movement
- Resistance to back-driving
- The ability to maintain pressure when the motor is not actively rotating

The L298N motor driver allows the ESP32 to reverse the motor direction by changing the logic level of the driver input pins.

A simplified control table is shown below:

| Command | Motor input 1 | Motor input 2 | Result |
|---|---:|---:|---|
| Move down | HIGH | LOW | Motor rotates forward |
| Move up | LOW | HIGH | Motor rotates in reverse |
| Stop | LOW | LOW | Motor stops |

The exact direction depends on the physical motor wiring and can be reversed in software if required.

---

## Software Structure

The repository contains two separate firmware programs.

### Remote Controller

The remote-controller firmware is responsible for:

- Configuring the ESP-NOW connection
- Reading the two push buttons
- Encoding the movement command
- Transmitting the command to the actuator controller
- Sending a stop command when no button is pressed

### Actuator Controller

The actuator-controller firmware is responsible for:

- Configuring the ESP-NOW receiver
- Receiving movement commands
- Setting the L298N direction pins
- Driving the motor upward or downward
- Stopping the motor when required

The software is intentionally separated because the two ESP32 boards perform different roles.

---

## Repository Structure

```text
quac-esp32-control/
├── documentation/
│   ├── activity-diagram.png
│   ├── system-architecture.png
│   └── wiring-diagram.png
├── images/
│   ├── electronics-prototype.jpg
│   ├── final-prototype.jpg
│   ├── integrated-electronics.jpg
│   └── remote-controller.jpg
├── software/
│   ├── actuator-controller/
│   │   └── actuator-controller.ino
│   └── remote-controller/
│       └── remote-controller.ino
├── README.md
└── LICENSE
```

- `documentation/` contains the wiring, architecture, and activity diagrams.
- `images/` contains photographs of the electronics and final prototype.
- `software/remote-controller/` contains the firmware for the wireless remote.
- `software/actuator-controller/` contains the firmware for the motor-control unit.
- `README.md` documents the embedded system.

---

## Setup

The project requires:

- Two compatible ESP32 development boards
- Arduino IDE or another compatible ESP32 development environment
- ESP32 Arduino board package
- ESP-NOW support included with the ESP32 framework

### 1. Prepare the ESP32 Boards

Identify which ESP32 will be used as:

- Remote controller
- Actuator controller

### 2. Configure the Receiver Address

The remote controller must be configured with the MAC address of the actuator ESP32.

Replace the placeholder address in the remote-controller firmware:

```cpp
uint8_t receiverAddress[] = {
    0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX
};
```

### 3. Upload the Actuator Firmware

Open:

```text
software/actuator-controller/actuator-controller.ino
```

Select the correct ESP32 board and serial port, then compile and upload the firmware.

### 4. Upload the Remote Firmware

Open:

```text
software/remote-controller/remote-controller.ino
```

Select the second ESP32 board and serial port, then compile and upload the firmware.

### 5. Test the Electronics Safely

Before connecting the actuator to a loaded mechanism:

1. Disconnect the motor from the mechanical load or ensure the actuator can move freely.
2. Confirm that the motor remains stopped when no button is pressed.
3. Test the upward command.
4. Test the downward command.
5. Confirm that the direction matches the intended actuator movement.
6. Confirm that the motor stops when the button is released.
7. Test communication loss and reset behaviour.

---

## Testing

<p align="center">
  <img src="images/electronics-prototype.jpg" alt="QuAC electronics prototype" width="550">
  <br>
  <em>Prototype electronics used during ESP-NOW and motor-control testing.</em>
</p>

The mechatronics system was developed and tested incrementally.

Testing included:

- Button input
- ESP-NOW communication
- Transmission of test values
- Motor direction
- Motor-driver response
- Remote operation
- Electronics integration
- Full-system testing with the actuator

The motor driver produced significant heat during testing. The module was therefore mounted with 3D-printed spacers to improve airflow and reduce direct contact with the baseplate.

The original concept also included potentiometer-based speed adjustment. This function was removed after the potentiometer failed late in the project and was not required for the prototype's core operation.

A force sensor and LCD display were considered but not implemented because of time, packaging, and integration constraints. PRODUKTUTVIKLING.pdf

---

## Results

The wireless control system successfully transmitted movement commands from the remote controller to the actuator controller.

The final system provided:

- Wireless upward and downward actuator control
- Bidirectional motor operation
- Remote control outside the patient's reach
- Integration of electronics with the custom linear actuator
- Operation of the completed prototype during testing

The complete mechanism reached a measured compression force of **45.8 kg**, compared with the prototype target of 40 kg. PRODUKTUTVIKLING.pdf

This result demonstrates the mechanical and electrical capability of the educational prototype only. It does not establish clinical safety, medical effectiveness, or regulatory compliance.

---

## Known Limitations and Next Steps

- The prototype does not include a dedicated emergency-stop mechanism.
- No limit switches are implemented to prevent excessive actuator travel.
- The motor driver developed significant heat during operation.
- The system uses separate power sources for the ESP32 electronics and motor supply.
- The original speed-control potentiometer was removed from the final prototype.
- No force sensor is included for closed-loop pressure control.
- The prototype has not been clinically validated and is not suitable for medical use.

Natural next steps would include:

- Adding upper and lower limit switches
- Adding a hardware emergency stop
- Implementing communication-loss fail-safe behaviour
- Replacing the L298N with a more efficient motor driver
- Using a regulated power system for both control electronics and motor supply
- Adding calibrated force sensing
- Implementing closed-loop force control
- Separating communication, motor control, and safety logic into modular source files

---

## Contributions

The complete QuAC prototype was developed by a twelve-person Mechanical Engineering student team at NTNU.

The mechatronics subgroup consisted of **Mohamed Elwalid Fadul** and **Hardik Deshpande**. The subgroup was responsible for the electronic components, programming, and integration of the mechatronic system into the physical prototype. PRODUKTUTVIKLING.pdf

My main contributions included:

- ESP32 implementation
- ESP-NOW communication
- Remote-controller development
- Motor-control logic
- Electronics wiring and testing
- Integration with the custom linear actuator
- Full-system testing and troubleshooting
