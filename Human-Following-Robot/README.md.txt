# Human Following Robot

An interactive robot designed to detect and follow a person or object using a combination of ultrasonic and infrared sensors.

## How It Works
The robot calculates the distance to the target using an ultrasonic sensor. If the target is between 5 cm and 20 cm away, all four motors drive forward to follow the subject. Two IR sensors mounted on the left and right help the robot steer; if the left or right IR sensor detects the target moving to the side, the robot adjusts its motor directions to turn and track the movement. If the target gets too close (under 5 cm) or moves out of range (over 20 cm), the motors stop to maintain a safe distance.

## Hardware Components
* Arduino Uno
* Adafruit Motor Shield (L293D)
* 4x DC Gear Motors
* HC-SR04 Ultrasonic Sensor
* 2x Infrared (IR) Obstacle Sensors
* Micro Servo Motor
* Chassis, wheels, and power supply

## Pin Configuration
| Component | Arduino Pin |
| :--- | :--- |
| Right IR Sensor | A5 |
| Left IR Sensor | A2 |
| Ultrasonic Trig | A3 |
| Ultrasonic Echo | A4 |
| Servo Motor | D10 |
| DC Motors 1-4 | Motor Shield Terminals (M1, M2, M3, M4) |

## Software & Libraries
Requires the following libraries:
* `AFMotor.h` (Adafruit Motor Shield)
* `NewPing.h` (Optimized Ultrasonic sensor library)
* `Servo.h`