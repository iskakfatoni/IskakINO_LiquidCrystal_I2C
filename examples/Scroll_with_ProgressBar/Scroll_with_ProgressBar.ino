/*
  Scroll_with_ProgressBar.ino
  ------------------------------------------------------------
  Advanced showcase example for IskakINO_LiquidCrystal_I2C

  Demonstrates:
  - Smooth progress bar using custom CGRAM characters
  - scrollDisplayLeft() for running text
  - Non-blocking update using millis()
  - Partial LCD refresh (no flicker)

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
  Version    : 1.0.1
*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(16, 2);

// Timing
unsigned long lastScroll = 0;
unsigned long lastProgress = 0;

// Progress state
uint8_t progress = 0;

// Custom characters (0–5 pixels)
const uint8_t bar0[8] = {0,0,0,0,0,0,0,0};
const uint8_t bar1[8] = {0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000};
const uint8_t bar2[8] = {0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000};
const uint8_t bar3[8] = {0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100};
const uint8_t bar4[8] = {0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110};
const uint8_t bar5[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};

void drawProgressBar(uint8_t percent, uint8_t width);

void setup() {
  lcd.begin();
  lcd.backlight();

  // Load CGRAM characters
  lcd.createChar(0, bar0);
  lcd.createChar(1, bar1);
  lcd.createChar(2, bar2);
  lcd.createChar(3, bar3);
  lcd.createChar(4, bar4);
  lcd.createChar(5, bar5);

  lcd.setCursor(0, 0);
  lcd.print("IskakINO LCD ");
  lcd.setCursor(0, 1);
}

void loop() {
  unsigned long now = millis();

  // Scroll text every 300 ms
  if (now - lastScroll >= 300) {
    lastScroll = now;
    lcd.scrollDisplayLeft();
  }

  // Update progress bar every 200 ms
  if (now - lastProgress >= 200) {
    lastProgress = now;

    drawProgressBar(progress, 16);
    progress++;

    if (progress > 100) {
      progress = 0;
    }
  }
}

// Draw smooth progress bar on row 1
void drawProgressBar(uint8_t percent, uint8_t width) {
  uint16_t pixels = (percent * width * 5) / 100;

  lcd.setCursor(0, 1);

  for (uint8_t i = 0; i < width; i++) {
    uint8_t level = (pixels >= 5) ? 5 : pixels;
    lcd.write(level);
    pixels = (pixels > 5) ? pixels - 5 : 0;
  }
}
