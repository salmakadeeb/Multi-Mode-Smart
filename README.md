# Multi-Mode Smart Car Robot
> Autonomous robot built on ATmega32 with 3 operating modes: Bluetooth control, obstacle avoidance, and line following.


---
<img width="1033" height="559" alt="image" src="https://github.com/user-attachments/assets/a70cdd7e-5246-4545-8d3b-56affa03756e" />

## 📌 Project Overview

A multi-functional smart car controlled by an **ATmega32 microcontroller**, capable of switching between three fully integrated operating modes:

| Mode | Type | Key Hardware |
|------|------|-------------|
| 🔵 Mode 1 | Bluetooth Control (manual) | HC-05, L298N, Android App |
| 🟡 Mode 2 | Obstacle Avoidance (autonomous) | HC-SR04, LCD, Buzzer |
| 🟢 Mode 3 | Line Following (autonomous) | IR Sensors (x2) |

---

## 🎥 Demo

> *(Add a GIF or YouTube link here showing all 3 modes in action)*

---

## ⚙️ System Architecture

```
[Phone App]
    │
    │ Bluetooth (HC-05)
    ▼
[ATmega32 @ 16MHz]
    ├──── UART (PD0/PD1) ──────── HC-05 Bluetooth Module
    ├──── PC0/PC1 ─────────────── HC-SR04 Ultrasonic (Trigger/Echo)
    ├──── PC2/PC3 ─────────────── IR Sensors (Left/Right)
    ├──── PORTA (D0–D7) ───────── LCD 16x2 (8-bit mode)
    ├──── PB0/PB1 ─────────────── LCD Control (RS/EN)
    ├──── PD2–PD5 ─────────────── L298N Motor Driver (IN1–IN4)
    ├──── PB3 (OC0/Timer0 PWM) ── Motor A Speed (ENA)
    ├──── PD7 (OC2/Timer2 PWM) ── Motor B Speed (ENB)
    └──── PC4 (BUZ) ───────────── Buzzer
```

---

## 🔧 Hardware Components

| Component | Model | Function |
|-----------|-------|----------|
| Microcontroller | ATmega32 (16MHz) | Central controller |
| Bluetooth Module | HC-05 | Wireless UART communication |
| Motor Driver | L298N (H-Bridge) | DC motor direction & speed |
| Ultrasonic Sensor | HC-SR04 | Distance measurement |
| IR Sensors | Generic (x2) | Line detection |
| Display | LCD 16x2 (LM016L) | System status & distance |
| Buzzer | Passive | Obstacle warning sound |
| DC Motors | Geared (x2) | Wheel drive |
| Power | 12V battery | Main supply |

---

## 📱 Mode 1 — Bluetooth Control

The car is manually controlled via a **custom Android application** over HC-05 Bluetooth.

### Commands sent from app to ATmega32:

| Command | Action |
|---------|--------|
| `F` | Move Forward |
| `B` | Move Backward |
| `L` | Turn Left |
| `R` | Turn Right |
| `S` | Stop |
| `1` / `2` / `3` / `4` | Gear Speed 1–4 |
| `+` / `-` | Speed Up / Speed Down |
| `M` | Switch to Manual mode |
| `A` | Switch to Auto (Obstacle) mode |
| `X` | Switch to Line Following mode |

### How it works:
- HC-05 receives commands from the phone via UART @ 9600 baud (8N1)
- ATmega32 reads UART data register (UDR)
- Command is parsed and passed to motor control functions
- PWM via Timer0 (OC0/PB3) and Timer2 (OC2/PD7) controls speed through L298N

---

## 🟡 Mode 2 — Obstacle Avoidance

The car drives autonomously and avoids obstacles without any user input.

### Logic:
```
IF distance > 30cm  →  Move Forward
IF distance < 30cm  →  Stop → Reverse → Turn Right
IF still blocked    →  Turn Left
IF distance < 20cm  →  Buzzer ON
```

### Distance calculation:
```c
// Send 10μs trigger pulse on PC0
// Measure echo pulse width on PC1
distance_cm = echo_time_us / 58;
// Timeout (no object): return 999
```

LCD displays real-time distance and current mode status.

---

## 🟢 Mode 3 — Line Following

The car follows a black line on a white surface using two IR sensors.

### Logic:
```
Both on line   →  Move Forward
Right off line →  Turn Left
Left off line  →  Turn Right
Both off line  →  Stop
```

IR sensors: LEFT = PC2, RIGHT = PC3

---

## 🧠 Firmware Details

### PWM Motor Speed Control (Timer0 & Timer2)

```c
// Timer0 — Motor A (Fast PWM, Non-Inverting, No Prescaler)
TCCR0 = (1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS00);
OCR0 = speed_value; // 0–255

// Timer2 — Motor B (same configuration)
TCCR2 = (1<<WGM21)|(1<<WGM20)|(1<<COM21)|(1<<CS20);
OCR2 = speed_value;

// Default speed = 180
```

### UART Initialization (HC-05 @ 9600 baud)

```c
// Baud rate = 9600, 8 data bits, 1 stop bit
UBRRL = 51; // for 8MHz clock
UCSRB = (1<<RXEN);
UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
```

### HC-05 Voltage Level Shifter
HC-05 RXD operates at 3.3V logic. ATmega32 TX (5V) is divided using:
- R1 = 2kΩ (series)
- R2 = 1kΩ (to GND)
- Output ≈ 3.3V → safe for HC-05 RXD

---

## 🖥️ Proteus Simulation

The full circuit was simulated in **Proteus 8** before hardware implementation:

- Mode 1: L298N + DC motors + HC-05 wiring
- Mode 2: HC-SR04 + LCD (LM016L) + Buzzer (BC547 transistor driver)
- Mode 3: Dual IR sensor inputs
- Full ATmega32 pin mapping validated in simulation

---

## 📂 Project Structure

```
smart-car-robot/
│
├── src/
│   ├── main.c               # Main loop + mode switching
│   ├── motor.c / motor.h    # PWM motor control (Timer0, Timer2)
│   ├── bluetooth.c / .h     # UART init + command reading
│   ├── ultrasonic.c / .h    # HC-SR04 distance measurement
│   ├── ir_sensor.c / .h     # IR sensor reading (PC2, PC3)
│   ├── lcd.c / lcd.h        # LCD 8-bit driver
│   
│
├── proteus/
│   └── smart_car.pdsprj     # Proteus simulation file
│
├── android-app/
│   └── SmartCarController.apk  # Android app (prebuilt)
│
└── README.md
```

---

## 🚀 How to Run

### Hardware:
1. Connect all components per the Proteus schematic
2. Flash `main.hex` to ATmega32 using AVR programmer (e.g. USBasp)
3. Power on the circuit (12V for motors, 5V regulated for ATmega32)
4. Pair phone with HC-05 (default PIN: `1234`)
5. Open the Android app → tap **Connect** → select your HC-05 device
6. Select mode from the app (MANUAL / AUTO / LINE)

### Simulation:
1. Open `proteus/smart_car.pdsprj` in Proteus 8
2. Load `main.hex` into the ATmega32 component
3. Run simulation and test each mode

---

## 👥 Team

| Name | Role |
|------|------|
| Salma Kadeeb | Bluetooth Module (UART firmware) |
| Alaa allah | Avoid Obstacle |
| Youstina Aziz | IR Sensor |
| Joseph Nader | Line Following |
| Youssef Gadallah | Hardware Circuit + Motor Module |
| Alla Saber | Ulterasonic + LCD |

**Supervisor:** Dr. Khaled Saada

**Teaching Assistant:** TA. Youssef El-Sheheimyq

**Course:** The AVR Microcontroller and Embedded Systems


---

## 🛠️ Tools & Technologies

![C](https://img.shields.io/badge/Language-C-blue)
![ATmega32](https://img.shields.io/badge/MCU-ATmega32-darkblue)
![Proteus](https://img.shields.io/badge/Simulation-Proteus%208-green)
![AVR](https://img.shields.io/badge/Architecture-AVR%208--bit-orange)
![Android](https://img.shields.io/badge/App-Android-brightgreen)

- **Language:** C (AVR-GCC)
- **MCU:** ATmega32 @ 16MHz
- **Simulation:** Proteus 8
- **Communication:** UART, Bluetooth (HC-05)
- **Motor Control:** L298N H-Bridge + Timer PWM
- **Sensors:** HC-SR04 Ultrasonic, IR reflective sensors
- **Display:** LCD 16x2
- **App:** Custom Android (Bluetooth Serial)

---

## 📄 License

This project is for educational purposes.
Feel free to use, modify, and build on it with attribution.

---

> Built as a final embedded systems project — Higher Institute of Engineering and Technology, Beheira · 2026
