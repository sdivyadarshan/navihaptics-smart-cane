# Design Decisions and Prototype Engineering Notes

## Single ultrasonic sensor on a servo

A single HC-SR04 sensor is mounted on a servo to inspect three directions. This avoids using three separate range sensors and keeps the sensing subsystem compact. The trade-off is sequential rather than simultaneous coverage.

## Three directional vibration motors

Separate left, forward, and right motors provide a tactile indication of the detected direction. Only one motor is activated during a lock. The motors are switched through BC547 transistor stages rather than being driven directly from Pico GPIO pins.

## Lock and release hysteresis

The firmware uses a 15 cm lock threshold and a 22 cm release threshold. Separate thresholds reduce unstable switching when readings fluctuate near one boundary.

## Servo hold behaviour

The acquired servo command is saved as `lockedAngle`. The scan index is not advanced in the locked state. This is intended to keep the sensor facing the direction that detected the close obstacle until the release condition is met.

A hobby servo holds position using its internal control loop while powered. Actual holding performance depends on the servo, load, supply voltage, and mechanical mounting.

## Alert behaviour

During normal scanning, the buzzer and all vibration motors remain OFF. During a lock, the buzzer is continuously HIGH and only the corresponding direction motor is HIGH. This is a deliberate change from continuous or distance-proportional vibration.

## Servo calibration

The assembled cane uses calibrated command angles of 10°, 50°, and 90° for left, forward, and right. The physical forward direction depends on servo horn alignment and sensor mounting. Recalibrate after changing the mounting geometry.

## Measurement sequencing

The servo is moved and allowed to settle before the ultrasonic reading is taken. This ensures that the measured direction corresponds to the commanded scan position.

## Electrical driver stages

Each vibration motor is controlled using a BC547 transistor and a 1 kΩ base resistor. A suppression diode is fitted across each motor in the prototype. The transistor pinout and diode current rating must be verified against the actual components.

## Limitations retained in this prototype

- A single ultrasonic sensor cannot measure all directions simultaneously.
- The HC-SR04 can produce invalid readings on angled, soft, narrow, or acoustically absorbent targets.
- A no-echo reading is treated as a release condition in the current firmware.
- The buzzer is assumed to be an active buzzer.
- The current code uses blocking waits and `pulseIn()`; scan responsiveness depends on echo timeout and servo settling time.
