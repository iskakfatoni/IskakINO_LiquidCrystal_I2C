/*
  Advanced_CustomChar_ProgressBar.ino
  ------------------------------------------------------------
  Advanced example for IskakINO_LiquidCrystal_I2C library

  Demonstrates:
  - Custom character (CGRAM) usage
  - Smooth progress bar using 5x8 dot characters
  - Non-blocking progress update (millis based)
  - LCD partial refresh (no flicker)

  LCD:
  - Type : HD44780 + I2C (PCF8574)
  - Size : 16x2

  Wiring:
  - SDA -> SDA
  - SCL -> SCL
  - VCC -> 5V / 3.3V
  - GND -> GND

  Created by : Iskak Fatoni
  Library    : IskakINO_LiquidCrystal_I2C
  Version    : 1.0.0
*/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(16, 2);

unsigned long lastUpdate = 0;
uint8_t progress = 0;

// Custom characters for progress bar (0–5 pixels)
byte bar0[8] = { 0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000 };
byte bar1[8] = { 0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000 };
byte bar2[8] = { 0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000 };
byte bar3[8] = { 0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100 };
byte bar4[8] = { 0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110 };
byte bar5[8] = { 0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111 };

void setup() {
  lcd.begin();
  lcd.backlight();

  // Load custom characters into CGRAM
  lcd.createChar(0, bar0);
  lcd.createChar(1, bar1);
  lcd.createChar(2, bar2);
  lcd.createChar(3, bar3);
  lcd.createChar(4, bar4);
  lcd.createChar(5, bar5);

  lcd.setCursor(0, 0);
  lcd.print("Loading...");

  lcd.setCursor(0, 1);
}

void loop() {
  unsigned long now = millis();

  // Update progress every 200 ms
  if (now - lastUpdate >= 200) {
    lastUpdate = now;

    drawProgressBar(progress, 16);
    progress++;

    if (progress > 100) {
      progress = 0;
      lcd.setCursor(0, 0);
      lcd.print("Restarting... ");
    }
  }
}

// Draw smooth progress bar
void drawProgressBar(uint8_t percent, uint8_t width) {
  uint16_t pixels = (percent * width * 5) / 100;

  lcd.setCursor(0, 1);

  for (uint8_t i = 0; i < width; i++) {
    uint8_t level = pixels >= 5 ? 5 : pixels;
    lcd.write(level);
    pixels = pixels > 5 ? pixels - 5 : 0;
  }
}
