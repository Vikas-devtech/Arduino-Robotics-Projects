# Obstacle Avoiding Robot

An autonomous 4-wheel drive robot that navigates environments by detecting and avoiding obstacles in real-time. 

## How It Works
The robot continuously drives forward until the ultrasonic sensor detects an obstacle within 35 cm. Upon detection, the robot stops, reverses slightly, and uses a servo motor to sweep the ultrasonic sensor left and right. It compares the distances and automatically turns toward the direction with the most open space.

## Hardware Components
* Arduino Uno (or compatible microcontroller)
* Adafruit Motor Shield (L293D)
* 4x DC Gear Motors
* HC-SR04 Ultrasonic Sensor
* Micro Servo Motor
* Chassis, wheels, and power supply

## Pin Configuration
| Component | Arduino Pin |
| :--- | :--- |
| Ultrasonic Trig | A3 |
| Ultrasonic Echo | A4 |
| Servo Motor | D10 |
| DC Motors 1-4 | Motor Shield Terminals (M1, M2, M3, M4) |

## Software & Libraries
Before uploading the `.ino` file, install the following libraries via the Arduino Library Manager:
* [Adafruit Motor Shield Library](https://learn.adafruit.com/adafruit-motor-shield/library-install) (`AFMotor.h`)
* [NewPing Library](https://github.com/livetronic/Arduino-NewPing) (`NewPing.h`)
* Built-in Servo Library (`Servo.h`)