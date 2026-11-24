# Automatic Street Light Control System Using LPC2129 ARM7

## 📌 Project Overview

The **Automatic Street Light Control System** is an embedded system developed using the LPC2129 ARM7 microcontroller. It automatically controls street lights based on ambient light intensity detected by an LDR sensor.

The system uses an ADC to convert the LDR's analog output into a digital value. The RTC manages time and date, while a 4×4 matrix keypad allows the user to configure time and street light schedules. A 16×2 LCD displays real-time information, including time, date, ADC value, and street light status.

## ✨ Features

- Automatic street light control based on ambient light intensity.
- LDR sensor interfaced with the ADC.
- RTC-based time and date management.
- User-configurable time and street light schedules.
- 4×4 matrix keypad for user input.
- 16×2 LCD for displaying system information.
- LED indicators to represent street light operation.
- Buzzer alarm indication.

## 🛠️ Hardware Components

- LPC2129 ARM7 Microcontroller
- LDR Sensor
- ADC
- RTC
- 4×4 Matrix Keypad
- 16×2 LCD Display
- LEDs
- Buzzer
- Power Supply

## 💻 Software Requirements

- Embedded C
- Keil µVision IDE
- Flash Magic

## ⚙️ Working Principle

1. The LDR senses the surrounding light intensity.
2. The ADC converts the analog sensor output into a digital value.
3. The LPC2129 compares the ADC value with a predefined threshold.
4. Based on the comparison, the street lights are switched ON or OFF.
5. The RTC maintains the current time and date.
6. The keypad allows the user to configure the RTC and street light schedule.
7. The LCD displays the current time, date, ADC value, and street light status.

## 📂 Project Structure

```text
Automatic-Street-Light-Control-System/
│
├── header_files/
│   ├── types.h
│   ├── defines.h
│   ├── delay.h
│   ├── LCD.h
│   ├── LCD_defines.h
│   ├── kpm.h
│   └── kpm_defines.h
│
├── source_files/
│   ├── main.c
│   ├── adc.c
│   ├── rtc.c
│   ├── LCD.c
│   ├── kpm.c
│   ├── delay.c
│   └── ext_proj.c
│
└── README.md
```

*Note: Update the folder and file names according to your actual project structure.*

## 🚀 Applications

- Automatic street lighting systems
- Smart city lighting
- Energy-efficient outdoor lighting
- Automatic lighting in public areas

## 🎯 Project Objective

To develop an intelligent street light control system that reduces unnecessary power consumption by automatically controlling street lights according to ambient light intensity and predefined time schedules.

## 👨‍💻 Development

Developed using **Embedded C** on the **LPC2129 ARM7 microcontroller**.
