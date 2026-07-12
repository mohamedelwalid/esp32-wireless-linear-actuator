# ESP32 Wireless Linear Actuator Control

Embedded software and electronics for the wireless control of a custom, DC motor-driven linear actuator developed as part of **TMM4121 – Product Development** at NTNU.

The system uses two ESP32 development boards communicating through **ESP-NOW**. A handheld controller reads two push buttons and a potentiometer, then transmits the input data to a second ESP32 that controls a 12 V DC gearmotor through an L298N motor driver.

> **Educational prototype:** This repository documents a student engineering prototype. The complete system is not a certified medical device and is not intended for clinical use.

My responsibilities were within **mechatronics and programming**, shared with one other team member, and included ESP32 implementation, ESP-NOW communication, motor-control logic, electronics integration, testing, and troubleshooting.

This repository covers only the **embedded software, electronics, wireless communication, and actuator-control system**. The wider product context, mechanical design, CAD, manufacturing, structural analysis, and product-development process are documented separately in the project case study on my portfolio website.

<p align="center">
  <img src="images/final-prototype.jpg" alt="Wireless linear actuator control system integrated into the final prototype" width="650">
  <br>
  <em>Wireless linear actuator control system integrated into the final prototype.</em>
</p>

---

## Overview

The system provides wireless, bidirectional control of a custom leadscrew-based linear actuator.

Two ESP32 boards perform separate roles:

- **Remote controller** — reads the operator inputs and transmits control data.
- **Actuator controller** — receives the data and controls the DC motor through an L298N H-bridge.

```text
Buttons and potentiometer
            ↓
       Remote ESP32
            ↓
         ESP-NOW
            ↓
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
- Upward, downward, and stop states
- Potentiometer input for motor-speed control
- Battery-powered remote and actuator units
- Integration with a custom linear actuator
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
| ESP32, remote unit | Reads the buttons and potentiometer and transmits data |
| ESP32, actuator unit | Receives data and controls the motor driver |
| Two push buttons | Command upward and downward actuator movement |
| Potentiometer | Provides an analogue motor-speed input |
| L298N dual H-bridge | Provides bidirectional DC motor control |
| 12 V DC gearmotor | Drives the leadscrew-based linear actuator |
| Battery pack | Powers the motor and motor driver |
| Power bank | Powers the ESP32 control electronics |

### ESP32 Pin Configuration

#### Remote Controller

| Function | GPIO | Configuration |
|---|---:|---|
| Potentiometer input | 18 | Analogue input |
| Down button | 16 | `INPUT_PULLUP` |
| Up button | 17 | `INPUT_PULLUP` |

The buttons use the ESP32's internal pull-up resistors and are therefore **active-low**:

```text
LOW  = button pressed
HIGH = button released
```

#### Actuator Controller

| Function | GPIO | Connection |
|---|---:|---|
| Motor enable / speed | 25 | L298N ENA |
| Motor direction input 1 | 32 | L298N IN1 |
| Motor direction input 2 | 33 | L298N IN2 |

### Remote-Controller Wiring

<p align="center">
  <img src="documentation/remote-wiring-diagram.svg" alt="Remote-controller wiring diagram" width="520">
  <br>
  <em>Wiring diagram for the ESP32 remote controller.</em>
</p>

### Actuator-Controller Wiring

<p align="center">
  <img src="documentation/actuator-wiring-diagram.svg" alt="Actuator-controller wiring diagram" width="760">
  <br>
  <em>Wiring diagram for the ESP32 actuator controller, L298N, and DC motor.</em>
</p>

---

## Firmware

The repository contains one firmware program for each ESP32.

### Remote Controller

```text
software/remote-controller/remote-controller.ino
```

The remote-controller firmware:

- Initialises ESP-NOW
- Registers the actuator ESP32 as a peer
- Reads the up and down buttons
- Reads the potentiometer input
- Stores the values in a shared message structure
- Transmits the message every 500 ms
- Reports the transmission status through the serial monitor

The transmitted data structure contains:

```cpp
typedef struct struct_message {
    int speedval;
    int downState;
    int upState;
} struct_message;
```

<p align="center">
  <img src="documentation/remote-activity-diagram.svg" alt="Remote-controller activity diagram" width="500">
  <br>
  <em>Remote-controller logic for reading user input and transmitting control data.</em>
</p>

### Actuator Controller

```text
software/actuator-controller/actuator-controller.ino
```

The actuator-controller firmware:

- Initialises ESP-NOW as a receiver
- Receives the shared message structure
- Extracts speed and button states
- Sets the L298N direction inputs
- Applies the received speed value to the enable pin
- Stops the motor when neither movement button is pressed

The current motor logic is:

| Input state | IN1 | IN2 | Result |
|---|---:|---:|---|
| Down button pressed | HIGH | LOW | Motor rotates forward |
| Up button pressed | LOW | HIGH | Motor rotates in reverse |
| Both buttons released | LOW | LOW | Motor stops |

<p align="center">
  <img src="documentation/actuator-activity-diagram.svg" alt="Actuator-controller activity diagram" width="720">
  <br>
  <em>Actuator-controller logic for receiving data and controlling motor direction.</em>
</p>

---

## Wireless Communication

The two ESP32 boards communicate using **ESP-NOW**, a direct wireless protocol supported by the ESP32.

ESP-NOW was selected because it allows short control messages to be transmitted directly between two known devices without requiring a router or external Wi-Fi network.

The sender transmits three values:

```text
Motor-speed value
Down-button state
Up-button state
```

The actuator controller receives the same message structure and uses the values directly in the motor-control logic.

The receiver ESP32 is identified by its MAC address in the remote-controller firmware:

```cpp
uint8_t broadcastAddress[] = {
    0x4C, 0x11, 0xAE, 0xB3, 0xA0, 0x20
};
```

---

## Testing and Results

The electronics were developed incrementally before being integrated into the complete prototype.

Testing covered:

- Digital button inputs
- Analogue potentiometer input
- ESP-NOW message transmission
- Sender and receiver pairing
- Motor direction control
- PWM speed control
- Motor-driver response
- Integration with the linear actuator
- Operation under mechanical load

The L298N motor driver produced significant heat during testing. It was therefore mounted on 3D-printed standoffs to improve airflow and reduce direct contact with the baseplate.

During a static load test, the complete actuator system produced a measured compression output equivalent to **45.8 kgf**, exceeding the 40 kgf engineering target.

> This was a prototype load test and not a clinical validation.

---

## Limitations and Next Steps

- **Communication loss:** The current firmware does not automatically stop the motor if ESP-NOW messages are no longer received. A receiver timeout should place the motor in a default stop state.
- **Actuator travel:** No upper or lower limit switches prevent mechanical over-travel. Hardware limits should be added.
- **Motor driver:** The L298N produced significant heat under sustained load. A more efficient motor driver would improve reliability and power efficiency.
- **Control feedback:** The system operates without force feedback. A calibrated force sensor could enable closed-loop force control.
- **Emergency stop:** The prototype does not include a dedicated hardware emergency-stop circuit.
- **Firmware robustness:** Button debouncing, command validation, and explicit handling of simultaneous button presses should be implemented.

---

## Repository Structure

```text
esp32-wireless-linear-actuator/
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

- `documentation/` contains the activity and wiring diagrams.
- `images/` contains photographs from prototyping and the completed system.
- `software/remote-controller/` contains the transmitter firmware.
- `software/actuator-controller/` contains the receiver and motor-control firmware.

---

## Setup

### Requirements

- Two ESP32 development boards
- Arduino IDE or another compatible ESP32 development environment
- ESP32 Arduino board package
- ESP-NOW support included with the ESP32 framework
- L298N motor driver
- Compatible DC motor and power supply

### Upload the Actuator Firmware

Open:

```text
software/actuator-controller/actuator-controller.ino
```

Select the correct ESP32 board and serial port, then compile and upload the firmware.

### Confirm the Receiver MAC Address

Verify that the MAC address in the remote-controller firmware matches the actuator ESP32:

```cpp
uint8_t broadcastAddress[] = {
    0x4C, 0x11, 0xAE, 0xB3, 0xA0, 0x20
};
```

### Upload the Remote Firmware

Open:

```text
software/remote-controller/remote-controller.ino
```

Select the second ESP32 board and serial port, then compile and upload the firmware.

### Initial Test

Before applying mechanical load:

1. Confirm that the motor remains stopped when both buttons are released.
2. Test the downward command.
3. Test the upward command.
4. Confirm that the movement direction matches the intended actuator direction.
5. Confirm that releasing the button stops the motor.
6. Verify the potentiometer input and motor-speed response.
7. Test the actuator without load before performing a loaded test.

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
