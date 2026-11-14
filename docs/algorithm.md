# Obstacle Detection and Feedback Algorithm

## 1. Scan positions

The firmware cycles through three calibrated servo commands:

- 10° — left
- 50° — forward
- 90° — right

At each position, the firmware waits for the servo to settle before taking a distance measurement.

## 2. Distance calculation

The HC-SR04 returns an ECHO pulse. The firmware measures its duration using `pulseIn()`.

Approximate distance:

```
distance_cm = echo_duration_us × 0.0343 / 2
```

If no echo is received before the timeout, the function returns -1 to indicate an invalid reading.

## 3. Lock and release thresholds

| Parameter | Value | Meaning |
|---|---:|---|
| LOCK_DISTANCE | 15 cm | Acquire obstacle lock at or below this distance |
| RELEASE_DISTANCE | 22 cm | Release only when distance is greater than this value |

The difference between the two thresholds is hysteresis. It reduces rapid lock/unlock changes when a measured object is near the boundary.

## 4. Scanning state

1. Ensure all vibration outputs and the buzzer are OFF.
2. Move the servo to the current scan angle.
3. Wait for the servo to settle.
4. Measure distance.
5. If a valid distance is greater than 15 cm, advance to the next scan direction.
6. If a valid distance is at or below 15 cm, save the current direction and angle and enter the locked state.

## 5. Locked state

1. Command the servo to the saved angle.
2. Keep the servo at that position; do not increment the scan index.
3. Activate the buzzer.
4. Activate only the vibration motor mapped to the locked direction.
5. Measure distance at the locked direction.
6. Remain locked while a valid measurement is at or below 22 cm.
7. Release when no echo is received or the measured distance is greater than 22 cm.
8. Turn all alert outputs OFF and resume scanning.

## 6. Pseudocode

```text
INITIALIZE pins, servo, serial output
SET servo to forward

REPEAT:
    IF state is LOCKED:
        hold servo at saved angle
        measure distance
        turn ON buzzer and selected direction motor

        IF no echo OR distance > release threshold:
            turn OFF all outputs
            clear lock
            resume scanning
        RETURN

    turn OFF all outputs
    move servo to current scan angle
    wait for servo to settle
    measure distance

    IF valid distance <= lock threshold:
        save angle and direction
        enter LOCKED state
        activate buzzer and selected motor
    ELSE:
        advance to next scan direction
```

## Design intent

Normal scanning is silent and produces no vibration. Feedback is event-driven: only a close obstacle causes the system to lock and alert. The directional motor communicates which scan sector detected the obstacle.
