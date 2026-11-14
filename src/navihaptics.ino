/*
  Navihaptics — Directional Obstacle Detection with Haptic Feedback
  Controller: Raspberry Pi Pico (RP2040)
  Framework: Arduino

  Behaviour:
  - Scan LEFT -> FORWARD -> RIGHT using one servo-mounted HC-SR04.
  - Keep all alerts OFF during normal scanning.
  - At or below LOCK_DISTANCE, hold the servo at the detected direction.
  - While locked, activate the buzzer and only that direction's vibrator.
  - Release only after distance exceeds RELEASE_DISTANCE or no echo is received.

  Pin numbers below are Raspberry Pi Pico GPIO numbers (GPx).
*/

#include <Servo.h>

#define TRIG_PIN       2    // GP2
#define ECHO_PIN       3    // GP3 (use 5 V-to-3.3 V level shifting!)
#define SERVO_PIN      4    // GP4

#define VIB_LEFT       6    // GP6, through BC547 driver
#define VIB_FRONT      7    // GP7, through BC547 driver
#define VIB_RIGHT      8    // GP8, through BC547 driver

#define BUZZER_PIN     15   // GP15

Servo scanServo;

// Calibrated servo commands: LEFT -> FORWARD -> RIGHT.
// These are servo command angles, not negative physical angles.
const int scanAngles[3] = {10, 50, 90};
const char *directionNames[3] = {"LEFT", "FORWARD", "RIGHT"};

const int LOCK_DISTANCE = 15;     // cm: acquire lock at or below this distance
const int RELEASE_DISTANCE = 22;  // cm: release only above this distance

const unsigned long SERVO_SETTLE_MS = 220;
const unsigned long ECHO_TIMEOUT_US = 25000UL;

int scanIndex = 0;
bool locked = false;
int lockedAngle = 50;
int lockedDirection = -1;

long getDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0) {
    return -1; // no valid echo
  }

  return (long)(duration * 0.0343f / 2.0f);
}

void allOutputsOff() {
  digitalWrite(VIB_LEFT, LOW);
  digitalWrite(VIB_FRONT, LOW);
  digitalWrite(VIB_RIGHT, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void activateLockedOutputs() {
  allOutputsOff();

  if (lockedDirection == 0) {
    digitalWrite(VIB_LEFT, HIGH);
  } else if (lockedDirection == 1) {
    digitalWrite(VIB_FRONT, HIGH);
  } else if (lockedDirection == 2) {
    digitalWrite(VIB_RIGHT, HIGH);
  }

  digitalWrite(BUZZER_PIN, HIGH);
}

void printReading(const char *state, int angle, long distance) {
  Serial.print(state);
  Serial.print(" | Direction: ");
  if (lockedDirection >= 0 && locked) {
    Serial.print(directionNames[lockedDirection]);
  } else {
    Serial.print(directionNames[scanIndex]);
  }
  Serial.print(" | Angle: ");
  Serial.print(angle);
  Serial.print(" | Distance: ");
  if (distance < 0) {
    Serial.println("NO ECHO");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(VIB_LEFT, OUTPUT);
  pinMode(VIB_FRONT, OUTPUT);
  pinMode(VIB_RIGHT, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  allOutputsOff();

  scanServo.attach(SERVO_PIN);
  scanServo.write(scanAngles[1]); // calibrated forward position
  delay(SERVO_SETTLE_MS);

  Serial.println("Navihaptics started");
  Serial.println("Normal scan: alerts OFF | Lock threshold: 15 cm | Release: >22 cm");
}

void loop() {
  // LOCKED STATE: do not advance the scan index or move the servo.
  if (locked) {
    scanServo.write(lockedAngle);
    long distance = getDistanceCm();
    activateLockedOutputs();

    Serial.print("LOCKED | Direction: ");
    Serial.print(directionNames[lockedDirection]);
    Serial.print(" | Distance: ");
    if (distance < 0) {
      Serial.println("NO ECHO");
    } else {
      Serial.print(distance);
      Serial.println(" cm");
    }

    if (distance < 0 || distance > RELEASE_DISTANCE) {
      locked = false;
      lockedDirection = -1;
      allOutputsOff();
      Serial.println("UNLOCKED | Resuming scan");
      delay(120);
    }

    return;
  }

  // SCANNING STATE: position first, allow it to settle, then measure.
  allOutputsOff();
  int currentAngle = scanAngles[scanIndex];
  scanServo.write(currentAngle);
  delay(SERVO_SETTLE_MS);

  long distance = getDistanceCm();

  Serial.print("SCAN | Direction: ");
  Serial.print(directionNames[scanIndex]);
  Serial.print(" | Angle: ");
  Serial.print(currentAngle);
  Serial.print(" | Distance: ");
  if (distance < 0) {
    Serial.println("NO ECHO");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }

  if (distance > 0 && distance <= LOCK_DISTANCE) {
    locked = true;
    lockedAngle = currentAngle;
    lockedDirection = scanIndex;

    scanServo.write(lockedAngle);
    activateLockedOutputs();

    Serial.print("LOCK ACQUIRED | ");
    Serial.println(directionNames[lockedDirection]);
    return;
  }

  scanIndex = (scanIndex + 1) % 3;
}
