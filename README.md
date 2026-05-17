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

---

# 2D 软体体素模块化机器人（中文版）

基于舵机与齿轮齿条机构驱动的 2D 软体体素模块化机器人系统。每个体素模块可主动伸缩，实现多模块协同运动。

## 仓库结构

```
2d soft voxel/
├── code/                          # Arduino 固件
│   ├── 16_module/                 # 16 路舵机协同正弦运动
│   └── certain_angle/             # 单路舵机角度校准工具
├── hardware model/                # 当前 SolidWorks 设计文件（v6）
│   ├── soft_voxel_newest version (v6)/   # 最新完整体素装配体
│   ├── battery and board assem/          # 电池仓 + 舵机驱动板安装座
│   └── cover molding/                    # 外壳零件及可打印 STL
├── print/                         # 当前版本可直接 3D 打印的 STL / 3MF 文件
├── figure and video/              # 硬件照片与演示视频
└── previous hardware model (OLD)/ # 设计历史：v1 → v5（仅供参考）
    ├── v2/ … v5/
    └── （散装文件）
```

## 硬件

### 关键组件
| 组件 | 说明 |
|------|------|
| 舵机 | AGFRC A06CLS V2（微型无铁芯舵机） |
| PWM 驱动板 | 815 16 路舵机驱动器（基于 PCA9685，I²C 地址 `0x41`） |
| 齿轮系统 | 金属齿轮齿条——20° 压力角（McMaster-Carr `3757N35`） |
| 微控制器 | Arduino 兼容板（已测试 Teensy / Arduino Nano） |

### 3D 打印（当前版本——v6）
所有可打印的 STL/3MF 文件均位于 `print/` 目录。主要零件：

| 文件 | 说明 |
|------|------|
| `entire cover.STL` / `Mirrorentire cover.STL` | 体素左/右侧外壳 |
| `gear rack_curve.STL` / `Mirrorgear rack_curve.STL` | 曲线齿条（镜像对） |
| `gear rack_curve_offset.STL` / `Mirrorgear rack_curve_offset.STL` | 偏置曲线齿条（镜像对） |
| `gear back.STL` | 齿轮背板支撑 |
| `servo_mount.STL` | 舵机安装支架 |
| `cover_mount.STL` | 外壳固定板 |
| `connector.STL` / `connector1.STL` | 模块间连接件 |
| `battery case.STL` / `battery cover.STL` / `battery switch cover.STL` | 电池仓组件 |
| `board support1.STL` | PCB 安装支撑 |
| `switch_v5.STL` | 电源开关壳体 |

### SolidWorks 源文件
`hardware model/soft_voxel_newest version (v6)/` 包含完整的参数化 SolidWorks 装配体（`all_assm_1.SLDASM`）及所有零件文件（`.SLDPRT`）。请使用 SolidWorks 2021 或更高版本打开 `all_assm_1.SLDASM`。

## 软件

### 依赖库
- [Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library) — 通过 Arduino Library Manager 安装

### `code/16_module` — 16 路舵机协同运动
通过 PCA9685 驱动板以正弦步态控制全部 16 路舵机：

| 参数 | 值 | 说明 |
|------|----|------|
| `PWM_FREQ` | 333 Hz | 舵机 PWM 频率 |
| `CENTER_ANGLE` | −45° | 振荡中心位置 |
| `AMPLITUDE` | 45° | 偏离中心的峰值幅度 |
| `FREQUENCY` | 1.0 Hz | 振荡频率 |

每路舵机有一个符号值（`servoSign[]`），决定其与波形同相伸出还是缩回，从而在 4×4 模块阵列中形成协同运动步态。

**接线：** 将 PCA9685 连接至微控制器的 I²C 引脚（SDA/SCL），板载地址为 `0x41`。

### `code/certain_angle` — 单路舵机校准
使用高分辨率 PWM（14 位，333 Hz）将引脚 `A3` 上的舵机设置到指定角度，用于装配前的行程校准和零位验证。

| 参数 | 值 |
|------|----|
| `TRIM_US` | 0.0 µs（可调整以修正摆臂偏差） |
| `NEUTRAL_US` | 1520 µs |
| `MIN_US` / `MAX_US` | 500 / 2500 µs |

以 115200 波特率打开串口监视器，即可查看每个设定角度对应的脉冲宽度。

## 设计历史

历次硬件迭代均保存在 `previous hardware model (OLD)/` 中，仅供参考：

| 版本 | 说明 |
|------|------|
| v1–v2 | 早期齿轮齿条概念验证 |
| v3 | 齿轮几何优化；引入独立舵机安装座 |
| v4 | 探索塑料齿轮方案；尝试磁性连接件 |
| v5 | 接近最终布局；集成电池与驱动板装配体 |
| v6 *（当前）* | 简化装配、改进外壳模具、采用镜像齿条几何 |

## 媒体资料

`figure and video/` 包含参考照片（`extended.jpg`、`shrinked.jpg`）及演示视频（`single module move.MOV`），展示单个体素模块的伸缩动作。

## 模块校准与装配指南

本指南规定了 **2d-soft-voxel** 机器人舵机校准与单模块装配的标准化流程。

为确保所有模块具备相同的运动学特性、可预测的行为和一致的结构，必须严格遵循以下硬件与软件分离工作流程。

---

### 阶段一：直接通过微控制器校准（无需 PCA9685）

在初始机械对准和单模块装配阶段，**禁止**使用 PCA9685 PWM 驱动板。

单模块舵机的校准与初步测试，必须将舵机信号线**直接**连接至 Arduino Nano ESP32 的指定 GPIO 引脚进行。

* **供电方式：** 仅通过计算机 USB 口为微控制器供电，暂勿接入任何外部电源。
* **软件控制：** 此阶段只使用 `certain_angle` 脚本/函数控制和校准舵机，暂勿运行复杂的多舵机协同代码。
* **原因：** 将系统隔离至最少的硬件和软件依赖，可大幅降低排障复杂度，并防止在结构对准阶段出现扩展板配置错误或 I2C 通信故障。

---

### 阶段二：逐步校准与装配

#### 步骤 1：舵机初始归零
在将舵机装入模块外壳之前，必须精确校准其花键位置，以避免装配后出现机械卡死或运动范围受限。
1. 将目标舵机直接连接至 Arduino Nano ESP32。
2. 运行 `certain_angle` 脚本，将舵机指令至基准校准位置（目标角度设为 **-90 度**）。
3. 确认舵机已完全到达、稳定并锁定在该位置后，再断开连接或进行机械安装。

#### 步骤 2：参考模块拆解与观察
若需复制现有参考模块或批量生产，以一个已验证正常的模块作为视觉基准：
1. 取一个运行正确的参考模块，仔细拆解。
2. 仔细观察并记录舵机摆臂（臂）与舵机本体在 -90 度基准位置时的**相对朝向和安装对准**关系。
3. 注意花键齿的精确啮合位置，确保新装配的模块不出现齿位偏移。

#### 步骤 3：校准后的模块装配
完成舵机归零和参考对准后，进行实物组装：
1. 将新舵机摆臂套入已校准的舵机轴，仔细复现步骤 2 中观察到的相对角度。
2. **确保内部零件一致性：** 最终紧固结构时，逐一核查各模块内部所有零件和连杆的相对位置完全相同、均匀一致。
3. 在保持对准的前提下拧紧所有安装螺钉，消除机械间隙、旋转松旷或结构偏移。

---

### ⚠️ 重要安全警告：供电顺序

> **停止：** 在**所有单模块**均通过 **Arduino Nano ESP32 的 USB 供电**完成校准、装配和验证之前，禁止进行整机测试或连接大电流电池。
>
> 连接电池前，务必先在 USB 安全电流限制下确认系统逻辑、控制指令及单路舵机动作均正常无误。若不严格遵守此供电顺序，一旦存在未发现的短路，将导致微控制器及周边电子元件永久损坏。
>
> *只有当所有的模块在仅仅通过 USB 给 Arduino Nano ESP32 供电调试好之后，才能去连接电池进行下一步。*

---

## 许可证

本项目供研究和教育用途开放使用。若您基于本设计进行二次开发，欢迎注明来源或致谢。
