# Limitations and Future Scope

## Current limitations

- One ultrasonic sensor scans directions sequentially; it does not provide continuous simultaneous coverage.
- Ultrasonic performance depends on target angle, surface, size, and environmental conditions.
- A no-echo reading currently releases a lock. In some environments, this may release even when an obstacle is still present.
- The prototype uses fixed servo angles and fixed distance thresholds.
- The current firmware uses blocking delays and `pulseIn()`, which limit responsiveness and multitasking.
- The cane has not been documented here as a certified or clinically evaluated assistive product.

## Potential engineering improvements

1. Add filtered or repeated distance measurements to reduce false triggers.
2. Add a configurable invalid-echo policy and sensor fault indication.
3. Evaluate time-of-flight sensors or multiple sensors for improved coverage.
4. Use a non-blocking state machine and timed servo scheduling.
5. Add battery monitoring and power-management circuitry.
6. Design a compact PCB and protected motor-driver stage.
7. Improve enclosure, cable strain relief, weather resistance, and serviceability.
8. Conduct supervised usability testing with informed consent and appropriate safety oversight.
9. Record detection range, false-positive rate, missed-obstacle cases, and battery runtime under repeatable conditions.

These are proposed future enhancements, not implemented features.
