# Testing and Validation

This document separates observed prototype behaviour from tests that still need to be recorded. Do not mark a test as passed without recording the actual setup and result.

## Bench checklist

| ID | Test | Expected behaviour | Result |
|---|---|---|---|
| T01 | Power-up with clear surroundings | Servo initializes forward; buzzer and all vibrators remain OFF | Not recorded |
| T02 | Serial Monitor at 115200 baud | Direction, angle, and distance readings are displayed | Not recorded |
| T03 | Place an object beyond 15 cm | Scanning continues; all alerts remain OFF | Not recorded |
| T04 | Place an object at or below 15 cm on left scan | Servo holds left command; buzzer and left motor activate | Not recorded |
| T05 | Place an object at or below 15 cm on forward scan | Servo holds forward command; buzzer and forward motor activate | Not recorded |
| T06 | Place an object at or below 15 cm on right scan | Servo holds right command; buzzer and right motor activate | Not recorded |
| T07 | Keep the object within 22 cm while locked | Servo remains at the saved direction and alerts remain active | Not recorded |
| T08 | Move the object beyond 22 cm | Outputs turn OFF and scanning resumes | Not recorded |
| T09 | Remove the echo while locked | Current firmware releases the lock | Not recorded |
| T10 | Repeat scans at each direction | Direction-to-motor mapping remains consistent | Not recorded |
| T11 | Operate servo and motors together | Pico does not reset; supply remains stable | Not recorded |
| T12 | Inspect ECHO electrical level | GP3 receives no more than 3.3 V | Must verify |

## Test record template

For each test, record:

- Date and location:
- Hardware revision:
- Firmware commit:
- Supply voltage:
- Object type and approximate dimensions:
- Measured distance:
- Servo direction:
- Buzzer result:
- Left motor result:
- Forward motor result:
- Right motor result:
- Pass / fail:
- Notes or observed limitations:

## Validation boundary

The prototype has been assembled and demonstrated by its student developer. This repository does not claim formal clinical testing, a controlled user study, quantified detection accuracy, environmental certification, or independent validation. Add measured results before making such claims.
