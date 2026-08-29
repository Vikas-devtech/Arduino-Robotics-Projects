#include <AFMotor.h>
#include <Servo.h>
#include <NewPing.h>

// ===================== MODES =====================
#define MODE_MANUAL  1
#define MODE_AUTO    2
#define MODE_SCAN    3

int currentMode = MODE_MANUAL;

// ===================== MOTOR SETUP =====================
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

// ===================== SERVO =====================
Servo scanServo;
#define SERVO_PIN 10
int servoCenter = 90;

// ===================== ULTRASONIC =====================
#define TRIG_PIN A3
#define ECHO_PIN A4
#define MAX_DISTANCE 200
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// ===================== BUZZER =====================
#define BUZZER_PIN A0

// ===================== VARIABLES =====================
char cmd = 'S';

int safeDistanceManual = 20;
int safeDistanceAuto   = 25;

// ===================== SETUP =====================
void setup() {

  Serial.begin(115200);

  // Servo
  scanServo.attach(SERVO_PIN);
  scanServo.write(servoCenter);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  // Motors
  stopMotors();
}

// ===================== LOOP =====================
void loop() {

  if (Serial.available()) {

    char incoming = Serial.read();

    // ===============================
    // MODES
    // ===============================

    if (incoming == 'M') {

      currentMode = MODE_MANUAL;

    }

    else if (incoming == 'A') {

      currentMode = MODE_AUTO;

    }

    else if (incoming == 'H') {

      currentMode = MODE_SCAN;

    }

    // ===============================
    // HORN
    // ===============================

    else if (incoming == 'K') {

      horn();

    }

    // ===============================
    // NORMAL COMMAND
    // ===============================

    else {

      cmd = incoming;

    }
  }

  // ===============================
  // MODE EXECUTION
  // ===============================

  if (currentMode == MODE_MANUAL)
    manualMode();

  else if (currentMode == MODE_AUTO)
    obstacleMode();

  else if (currentMode == MODE_SCAN)
    scanMode();
}

// =====================================================
// HORN
// =====================================================

void horn() {

  // Short car-horn style:
  // Low → high → low

  tone(BUZZER_PIN, 500);
  delay(180);

  tone(BUZZER_PIN, 350);
  delay(180);

  noTone(BUZZER_PIN);
}

// ===================== MANUAL MODE =====================
void manualMode() {

  // Servo head
  if (cmd == 'U')
    scanServo.write(60);

  else if (cmd == 'C')
    scanServo.write(servoCenter);

  else if (cmd == 'D')
    scanServo.write(120);

  // ===============================
  // FORWARD
  // ===============================

  if (cmd == 'F') {

    scanServo.write(servoCenter);
    delay(60);

    int frontDist = readDistance();

    if (frontDist <= safeDistanceManual) {

      moveBackward(110);
      delay(300);

      stopMotors();
      delay(150);

      int rightDist = lookRight();
      int leftDist  = lookLeft();

      // If all sides blocked → 180°
      if (frontDist <= safeDistanceManual &&
          leftDist  <= safeDistanceManual &&
          rightDist <= safeDistanceManual) {

        turnRight(150);
        delay(900);

        stopMotors();
        delay(300);
      }

      return;
    }

    moveForward(130);
  }

  // ===============================
  // BACKWARD
  // ===============================

  else if (cmd == 'B') {

    moveBackward(100);

  }

  // ===============================
  // LEFT
  // ===============================

  else if (cmd == 'L') {

    turnLeft(160);

  }

  // ===============================
  // RIGHT
  // ===============================

  else if (cmd == 'R') {

    turnRight(160);

  }

  // ===============================
  // STOP
  // ===============================

  else {

    stopMotors();

  }
}

// ===================== OBSTACLE MODE =====================
void obstacleMode() {

  scanServo.write(servoCenter);
  delay(40);

  int frontDist = readDistance();

  // ---------- CLEAR PATH ----------
  if (frontDist > safeDistanceAuto) {

    if (frontDist < 50)
      moveForward(85);

    else
      moveForward(100);

    return;
  }

  // ---------- OBSTACLE DETECTED ----------

  stopMotors();
  delay(120);

  // Step back
  moveBackward(110);
  delay(300);

  stopMotors();
  delay(150);

  // Scan sides
  int rightDist = lookRight();
  int leftDist  = lookLeft();

  // ===== ALL SIDES BLOCKED =====

  if (frontDist <= safeDistanceAuto &&
      leftDist  <= safeDistanceAuto &&
      rightDist <= safeDistanceAuto) {

    turnRight(160);
    delay(450);

    stopMotors();
    delay(120);

    turnRight(160);
    delay(450);

    stopMotors();
    delay(300);

    return;
  }

  // ===== TURN TOWARD OPEN SIDE =====

  if (leftDist > rightDist) {

    turnLeft(160);
    delay(280);

    stopMotors();
    delay(150);

  }

  else {

    turnRight(160);
    delay(280);

    stopMotors();
    delay(150);
  }
}

// ===================== SCAN MODE =====================
void scanMode() {

  static unsigned long lastMove = 0;
  static bool direction = false;

  if (millis() - lastMove > 600) {

    lastMove = millis();

    if (direction)
      scanServo.write(40);

    else
      scanServo.write(140);

    direction = !direction;
  }

  stopMotors();
}

// ===================== SENSOR FUNCTIONS =====================

int readDistance() {

  int total = 0;
  int valid = 0;

  for (int i = 0; i < 3; i++) {

    int cm = sonar.ping_cm();

    if (cm > 5 && cm < 200) {

      total += cm;
      valid++;

    }

    delay(5);
  }

  if (valid == 0)
    return 200;

  return total / valid;
}

// ===================== LOOK RIGHT =====================

int lookRight() {

  scanServo.write(40);

  delay(250);

  int d = readDistance();

  scanServo.write(servoCenter);

  delay(120);

  return d;
}

// ===================== LOOK LEFT =====================

int lookLeft() {

  scanServo.write(140);

  delay(250);

  int d = readDistance();

  scanServo.write(servoCenter);

  delay(120);

  return d;
}

// ===================== MOTOR FUNCTIONS =====================

void moveForward(int spd) {

  setSpeedAll(spd);
  runAll(FORWARD);
}

// ===================== MOVE BACKWARD =====================

void moveBackward(int spd) {

  setSpeedAll(spd);
  runAll(BACKWARD);
}

// ===================== TURN LEFT =====================

void turnLeft(int spd) {

  setSpeedAll(spd);

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);

  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

// ===================== TURN RIGHT =====================

void turnRight(int spd) {

  setSpeedAll(spd);

  motor1.run(FORWARD);
  motor2.run(FORWARD);

  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

// ===================== STOP MOTORS =====================

void stopMotors() {

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

// ===================== SET SPEED =====================

void setSpeedAll(int spd) {

  motor1.setSpeed(spd);
  motor2.setSpeed(spd);
  motor3.setSpeed(spd);
  motor4.setSpeed(spd);
}

// ===================== RUN ALL =====================

void runAll(int dir) {

  motor1.run(dir);
  motor2.run(dir);
  motor3.run(dir);
  motor4.run(dir);
}