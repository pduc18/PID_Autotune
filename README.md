
# PID_Autotune

A lightweight C implementation of PID auto-tuning using the Ziegler–Nichols method, designed for STM32. This project helps automatically calculate optimal PID coefficients (`Kp`, `Ki`, `Kd`) for controlling DC motors using encoder feedback and PWM output.

## 📦 Features

- 🔧 **Auto-tune PID** using the relay-based Ziegler–Nichols method.
- 📈 Supports real-time **position control** with encoder feedback.
- 🧠 Digital **incremental PID controller** implementation.
- ⚙️ Targeted for **STM32 microcontrollers** (HAL-based).
- 📤 PID parameters can be sent via UART for logging/debugging.

## 🧩 Folder Structure

```
PID_Autotune/
├── Core/
│   ├── Inc/
│   │   └── PID_AutoTune.h
│   └── Src/
│       ├── main.c       
│       └── PID_AutoTune.c        # Relay-based autotune algorithm
├── README.md
├── Makefile
└── ...
```

## 🚀 Getting Started

### 🛠 Requirements

- STM32 microcontroller (e.g., STM32F103, STM32F4xx, etc.)
- Encoder-based DC motor
- STM32CubeIDE or Makefile + HAL drivers
- UART or ST-Link for debugging/logging

### 🔄 How It Works

1. System toggles control output in relay-like fashion.
2. Measures oscillation amplitude and period.
3. Calculates ultimate gain `Ku` and period `Tu`.
4. Applies Ziegler–Nichols formulas to compute `Kp`, `Ki`, `Kd`.
5. Switches to real PID control with computed values.

### 🧪 Ziegler–Nichols Tuning Rules

| Controller | Kp       | Ki             | Kd            |
|------------|----------|----------------|----------------|
| P          | 0.5·Ku   | —              | —              |
| PI         | 0.45·Ku  | 1.2·Kp/Tu      | —              |
| PID        | 0.6·Ku   | 2·Kp/Tu        | Kp·Tu/8        |

## ⚙️ Example Usage

In `main.c`:

```c
PIDAutotuner_Init(&autotuner, 2.0f, 0.1f, 3000, -12.0f, 12.0f);

if (PIDAutotuner_Runtime(&autotuner, feedback_position)) {
    float Kp = autotuner.Kp;
    float Ki = autotuner.Ki;
    float Kd = autotuner.Kd;

    PID_Init(&pid, Kp, Ki, Kd, 0.001f); // 1 ms sampling
}
```

## 📡 UART Debug Output (optional)

You can print the tuning results for logging:

```c
printf("PID Autotune complete:\r\n");
printf("Kp = %.3f, Ki = %.3f, Kd = %.3f\r\n", autotuner.Kp, autotuner.Ki, autotuner.Kd);
```
