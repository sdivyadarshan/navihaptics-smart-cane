# Hardware, Components and Wiring

## Bill of materials

| Component | Quantity | Role |
|---|---:|---|
| Raspberry Pi Pico (RP2040) | 1 | Main controller |
| HC-SR04 ultrasonic sensor | 1 | Distance measurement |
| SG90 servo motor | 1 | Sensor direction positioning |
| Vibration motors | 3 | Left, forward, right haptic outputs |
| BC547 NPN transistor | 3 | Low-side motor switching |
| 1 kΩ resistor | 3 | Transistor base-current limiting |
| 1N4148 diode | 3 | Diodes fitted in the prototype |
| Active buzzer | 1 | Audible alert during lock |
| Jumper wires / prototyping board | As required | Interconnection |
| Regulated power supply | As required | Power for controller and loads |

## GPIO mapping

| Function | Pico pin |
|---|---|
| HC-SR04 TRIG | GP2 |
| HC-SR04 ECHO | GP3 through a 5 V-to-3.3 V level shifter/divider |
| Servo signal | GP4 |
| Left vibration driver input | GP6 |
| Forward vibration driver input | GP7 |
| Right vibration driver input | GP8 |
| Active buzzer control | GP15 |

See [pinout.csv](pinout.csv) for the machine-readable pin list.

## HC-SR04 ultrasonic sensor

| HC-SR04 | Connection |
|---|---|
| VCC | Regulated 5 V |
| GND | Common GND |
| TRIG | Pico GP2 |
| ECHO | Pico GP3 **through a voltage divider or logic-level shifter** |

**Important:** HC-SR04 ECHO is normally a 5 V logic signal. Raspberry Pi Pico GPIO is not 5 V tolerant. Do not connect ECHO directly to GP3. Use a suitable divider or level shifter that brings the signal to 3.3 V.

## Servo motor

| Servo lead | Connection |
|---|---|
| Red | Regulated 5 V supply |
| Brown / black | Ground |
| Orange / yellow | Pico GP4 signal |

The servo should use a supply capable of handling its startup and stall current. Join the servo supply ground to Pico GND. Avoid powering a loaded servo from the Pico 3.3 V output.

## Vibration motor driver (repeat for each direction)

Each vibration motor is switched using one BC547 NPN transistor.

- Pico GPIO (GP6 / GP7 / GP8) → 1 kΩ resistor → BC547 base.
- BC547 emitter → common GND.
- BC547 collector → vibration motor negative terminal.
- Vibration motor positive terminal → suitable motor supply positive.
- Motor supply ground → common GND.

**Check the transistor pinout from the exact BC547 manufacturer's datasheet.** With the flat face toward you, many common BC547 packages are C-B-E from left to right, but package/vendor variations must be checked before wiring.

### Motor suppression diode

A diode is placed across each brushed vibration motor to clamp the inductive voltage spike generated when the motor is switched off. The cathode (striped end) faces motor positive; the anode faces the transistor/ motor-negative side.

The prototype parts list records 1N4148 diodes. For a motor load, a suitably rated rectifier or Schottky diode (for example, 1N400x where its switching characteristics and current rating are suitable) is generally a more robust choice. Verify the actual motor current and diode rating.

## Buzzer

The sketch assumes a low-current **active buzzer** controlled by GP15:

- Buzzer positive → GP15 only if its current and voltage requirements are within the GPIO limits.
- Buzzer negative → GND.

If the buzzer draws more current than a GPIO can safely provide, use a transistor driver and an appropriate supply. Confirm whether the buzzer is active or passive; a passive buzzer needs a tone/PWM signal rather than a steady HIGH.

## Power and grounding

- All supply grounds must be connected together (common ground).
- Keep motor/servo current out of the Pico 3.3 V regulator.
- Use a regulated supply with sufficient current for servo and motors.
- Do not connect a 5 V signal directly to a Pico GPIO.
- Check polarity and continuity before powering the prototype.

## Important prototype note

The earlier prototype wiring description used Pico VBUS for the servo and ultrasonic sensor and 3V3 OUT for the vibration motor supply. The final supply arrangement should be checked against the actual motor and servo ratings. The repository documents the functional signal mapping, but the power source must be sized and wired safely for the specific parts fitted.
