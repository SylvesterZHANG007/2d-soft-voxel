# 2D Soft Voxel Modular Robot

A 2D soft voxel modular robot system driven by servo motors and gear-rack mechanisms. Each voxel module can actively extend and retract, enabling coordinated multi-module locomotion patterns.

## Repository Structure

```
2d soft voxel/
├── code/                          # Arduino firmware
│   ├── 16_module/                 # 16-servo coordinated sinusoidal motion
│   └── certain_angle/             # Single-servo angle calibration utility
├── hardware model/                # Current SolidWorks design files (v6)
│   ├── soft_voxel_newest version (v6)/   # Latest full voxel assembly
│   ├── battery and board assem/          # Battery housing + servo driver board mount
│   └── cover molding/                    # Cover parts with print-ready STLs
├── print/                         # STL / 3MF files ready for 3D printing (current version)
├── figure and video/              # Photos and demo videos of the hardware
└── previous hardware model (OLD)/ # Design history: v1 → v5 (reference only)
    ├── v2/ … v5/
    └── (loose files)
```

## Hardware

### Key Components
| Component | Description |
|-----------|-------------|
| Servo | AGFRC A06CLS V2 (micro coreless servo) |
| PWM Driver | 815 16-Channel Servo Driver (PCA9685-based, I²C address `0x41`) |
| Gear System | Metal gear rack — 20° pressure angle (McMaster-Carr `3757N35`) |
| Microcontroller | Arduino-compatible board (Teensy / Arduino Nano tested) |

### 3D Printing (current version — v6)
All print-ready STL/3MF files are in `print/`. Key parts:

| File | Description |
|------|-------------|
| `entire cover.STL` / `Mirrorentire cover.STL` | Left/right voxel outer covers |
| `gear rack_curve.STL` / `Mirrorgear rack_curve.STL` | Curved gear racks (mirrored pair) |
| `gear rack_curve_offset.STL` / `Mirrorgear rack_curve_offset.STL` | Offset gear racks (mirrored pair) |
| `gear back.STL` | Rear gear support |
| `servo_mount.STL` | Servo mounting bracket |
| `cover_mount.STL` | Cover attachment plate |
| `connector.STL` / `connector1.STL` | Inter-module connectors |
| `battery case.STL` / `battery cover.STL` / `battery switch cover.STL` | Battery enclosure |
| `board support1.STL` | PCB mounting support |
| `switch_v5.STL` | Power switch housing |

### SolidWorks Source Files
`hardware model/soft_voxel_newest version (v6)/` contains the full parametric SolidWorks assembly (`all_assm_1.SLDASM`) and all constituent part files (`.SLDPRT`). Open `all_assm_1.SLDASM` in SolidWorks 2021 or later.

## Software

### Dependencies
- [Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library) — install via Arduino Library Manager

### `code/16_module` — Coordinated 16-Servo Motion
Controls all 16 servos through the PCA9685 driver with a sinusoidal gait:

| Parameter | Value | Description |
|-----------|-------|-------------|
| `PWM_FREQ` | 333 Hz | Servo PWM frequency |
| `CENTER_ANGLE` | −45° | Center position of oscillation |
| `AMPLITUDE` | 45° | Peak deviation from center |
| `FREQUENCY` | 1.0 Hz | Oscillation frequency |

Each servo has a sign (`servoSign[]`) that determines whether it extends or retracts in phase with the wave, creating a coordinated locomotion pattern across the 4×4 grid of modules.

**Wiring:** Connect PCA9685 to the microcontroller's I²C pins (SDA/SCL). Board address is `0x41`.

### `code/certain_angle` — Single-Servo Calibration
Sets one servo (pin `A3`) to a fixed angle using high-resolution PWM (14-bit, 333 Hz). Useful for calibrating trim and verifying servo range before assembly.

| Parameter | Value |
|-----------|-------|
| `TRIM_US` | 0.0 µs (adjust to correct arm offset) |
| `NEUTRAL_US` | 1520 µs |
| `MIN_US` / `MAX_US` | 500 / 2500 µs |

Open Serial Monitor at 115200 baud to see the computed pulse width for each set angle.

## Design History

Previous hardware iterations are preserved in `previous hardware model (OLD)/` for reference:

| Version | Notes |
|---------|-------|
| v1–v2 | Early gear-rack concepts |
| v3 | Refined gear geometry; separate servo mount introduced |
| v4 | Alternative gear set with plastic gears; magnet-based connectors explored |
| v5 | Near-final layout; battery + board assembly integrated |
| v6 *(current)* | Streamlined assembly, improved cover molding, mirrored rack geometry |

## Media

`figure and video/` contains reference photos (`extended.jpg`, `shrinked.jpg`) and a demo video (`single module move.MOV`) showing a single voxel extending and retracting.

## Module Calibration and Assembly Guide

This guide outlines the standardized protocol for calibrating the servo motors and assembling the individual modules for the **2d-soft-voxel** robot.

To ensure identical kinematics, predictable behavior, and structural consistency across all modules, you must follow the precise hardware and software separation workflow described below.

---

### Phase 1: Direct Microcontroller Calibration (No PCA9685 Required)

During this initial mechanical alignment and single-module assembly phase, **DO NOT** use the PCA9685 PWM driver board.

The calibration and initial testing of individual module servos must be performed by connecting the servo signal lines **directly** to the designated GPIO pins of the Arduino Nano ESP32.

* **Power Source:** Power the microcontroller solely via the computer's USB port. Do not connect any external power supplies yet.
* **Software Control:** At this stage, exclusively use the `certain_angle` script/function to command and calibrate the servos. Do not attempt to run complex multi-servo coordination codes yet.
* **Why this approach?** This isolates the system to minimal hardware and software dependencies, drastically reducing troubleshooting complexity and preventing configuration or I2C communication errors on the expansion board during structural alignment.

---

### Phase 2: Step-by-Step Calibration and Assembly

#### Step 1: Servo Initial Zeroing
Before integrating any servo motor into a module casing, its spline position must be precisely zeroed to avoid mechanical binding or restricted range of motion post-assembly.
1. Connect the target servo directly to the Arduino Nano ESP32.
2. Run the `certain_angle` script to command the servo to its baseline calibration position (set the target angle to **-90 degrees**).
3. Ensure the servo motor has fully reached, settled, and locked into this position before disconnecting the setup or moving to mechanical installation.

#### Step 2: Reference Deconstruction and Observation
If you are replicating an existing reference module or scaling up production, use a proven module as your visual anchor:
1. Take an existing, correctly operating reference module and carefully disassemble/deconstruct it.
2. Closely observe and document the exact **relative orientation and mounting alignment** between the servo horn (arm) and the servo body at the -90-degree baseline.
3. Note the precise spline tooth engagement to ensure the newly assembled modules do not suffer from a gear-tooth offset.

#### Step 3: Calibrated Module Assembly
With the servo zeroed and the reference alignment understood, proceed to physical fabrication:
1. Mate the new servo horn onto the newly calibrated servo shaft, carefully duplicating the relative angle observed during the reference deconstruction in Step 2.
2. **Ensure Internal Component Consistency**: During final structural fastening, double-check that the relative positions of all internal components and linkages across different modules are completely identical and uniform.
3. Securely tighten all mounting screws while maintaining this alignment to eliminate any mechanical backlash, rotational slop, or structural shifting.

---

### ⚠️ CRITICAL SAFETY WARNING: POWER SEQUENCE

> **STOP:** Do not proceed to full-system testing or connect the high-current battery source until **all individual modules** have been successfully calibrated, assembled, and verified using **ONLY USB power** (via the Arduino Nano ESP32).
>
> Always verify that the system logic, control commands, and individual servo movements function flawlessly under safe USB power limits before connecting the battery. Failure to strictly follow this power sequence can result in permanent damage to the microcontroller and surrounding electronics in the event of an undetected short circuit.
>
> *只有当所有的模块在仅仅通过 USB 给 Arduino Nano ESP32 供电调试好之后，才能去连接电池进行下一步。*

---

## License

This project is released for research and educational use. If you build on this design, a citation or acknowledgment is appreciated.
