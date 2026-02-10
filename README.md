# IskakINO_LiquidCrystal_I2C

LiquidCrystal I2C library for HD44780 LCD with **automatic I2C address detection**.  
Designed to be **backward compatible**, lightweight, and friendly for Arduino & ESP32 projects.

---

## ✨ Features
- Automatic I2C address scanning (PCF8574)
- Backward compatible with `LiquidCrystal_I2C`
- Optional splash screen
- Compile-time configuration
- Arduino & ESP32 compatible
- Lightweight and optimized for flash & RAM

---

## 📦 Installation
**Arduino IDE**
1. Download this repository as ZIP
2. Open Arduino IDE
3. Sketch → Include Library → Add .ZIP Library

---

## 🚀 Basic Usage
```cpp
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(16, 2);

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("Hello IskakINO");
}

void loop() {
}
