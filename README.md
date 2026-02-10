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
```

## 🎬 Showcase Examples

Library ini menyediakan example **dari basic hingga advanced**,  
dirancang untuk menunjukkan **penggunaan LCD yang benar, efisien, dan non-blocking**.

### 📌 Basic
- **Basic**  
  Menampilkan teks sederhana di LCD dan memastikan wiring serta alamat I2C terdeteksi otomatis.

### ⚙️ Intermediate
- **Scroll_and_Autoscroll**  
  Demonstrasi:
  - `scrollDisplayLeft()`  
  - `scrollDisplayRight()`  
  - `autoscroll()`  
  - `leftToRight()` / `rightToLeft()`

- **Ticker_Text**  
  Running text (ticker) halus berbasis `millis()`  
  tanpa `delay()` dan tanpa `lcd.clear()`.

### 🚀 Advanced
- **Advanced_CustomChar_ProgressBar**  
  Penggunaan **custom character (CGRAM)** untuk membuat progress bar halus tanpa flicker.

- **Scroll_with_ProgressBar**  
  Showcase UI LCD yang menggabungkan:
  - scrolling text  
  - progress bar  
  - non-blocking update  
  Cocok untuk dashboard IoT ESP32.

### 📂 Struktur Folder Example
examples/
├── Basic/
├── Scroll_and_Autoscroll/
├── Ticker_Text/
├── Advanced_CustomChar_ProgressBar/
└── Scroll_with_ProgressBar/


## 🔁 API Compatibility

Library ini dirancang **100% backward compatible** dengan API populer:

LiquidCrystal_I2C lcd(address, columns, rows);

atau mode auto-detect:

LiquidCrystal_I2C lcd(columns, rows);

### ✅ Kompatibel dengan:
- print(), setCursor(), clear(), home()  
- cursor(), noCursor()  
- blink(), noBlink()  
- scrollDisplayLeft(), scrollDisplayRight()  
- autoscroll(), noAutoscroll()  
- leftToRight(), rightToLeft()  
- createChar() (custom character / CGRAM)

⚠️ **Catatan:**  
API lama tetap berfungsi tanpa perubahan kode.


## 🏷 Versioning

Library ini menggunakan **Semantic Versioning**:

MAJOR.MINOR.PATCH

### 📌 Versi Saat Ini
- **v1.0.0**
  - Initial stable release  
  - Backward compatible API  
  - Auto I2C address detection  
  - Custom character support  
  - Scroll, cursor, blink, progress bar examples

Perubahan besar (breaking change) **tidak akan dilakukan** pada versi 1.x.


## 📜 License

This library is released under the **MIT License**.

You are free to:  
- use  
- modify  
- distribute  
- include in commercial projects  

As long as the original copyright notice is retained.
