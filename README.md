# IskakINO_LiquidCrystal_I2C

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue.svg)](https://www.arduino.cc/reference/en/libraries/)
[![Platform](https://img.shields.io/badge/platform-Arduino%20%7C%20ESP32-green)](#)
[![License](https://img.shields.io/badge/license-MIT-lightgrey)](LICENSE)
[![Version](https://img.shields.io/badge/version-1.0.1-orange)](#)

LiquidCrystal I2C library for **HD44780 LCD** using **PCF8574 I2C expander**  
with **automatic I2C address detection**.

Designed to be:
- ✅ Backward compatible with `LiquidCrystal_I2C`
- ⚡ Lightweight & optimized for flash/RAM
- 🔌 Arduino & ESP32 friendly
- 🧼 Clean API & maintainer-grade structure

---

## 📑 Table of Contents
- [Features](#-features)
- [Supported Hardware](#-supported-hardware)
- [Installation](#-installation)
- [Wiring](#-wiring)
- [Basic Usage](#-basic-usage)
- [Showcase Examples](#-showcase-examples)
- [API Compatibility](#-api-compatibility)
- [Versioning](#-versioning)
- [License](#-license)

---

## ✨ Features
- Automatic I2C address scanning (0x20–0x27)
- Full **HD44780 command support**
- Standard `LiquidCrystal` compatible API
- Custom character (CGRAM) support
- Scroll, autoscroll, cursor & blink control
- Non-blocking friendly (millis-based)
- Optimized for ESP32 & Arduino boards

---

## 🧩 Supported Hardware
- HD44780 compatible LCD (16x2, 20x4)
- I2C backpack using **PCF8574 / PCF8574A**
- Arduino UNO / Nano / Mega
- ESP32 / ESP8266

---

## 📦 Installation

### Arduino IDE
1. Open **Arduino IDE**
2. Sketch → Include Library → **Manage Libraries**
3. Search for **IskakINO_LiquidCrystal_I2C**
4. Click **Install**

### Manual (ZIP)
1. Download this repository as ZIP
2. Arduino IDE → Sketch → Include Library → Add `.ZIP Library`

---

## 🔌 Wiring

| LCD I2C | Arduino / ESP32 |
|------|----------------|
| SDA | SDA |
| SCL | SCL |
| VCC | 5V / 3.3V |
| GND | GND |

> ESP32 default: SDA = GPIO21, SCL = GPIO22

---

## 🚀 Basic Usage

```cpp
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(16, 2);

void setup() {
  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Hello IskakINO");
}

void loop() {
}
