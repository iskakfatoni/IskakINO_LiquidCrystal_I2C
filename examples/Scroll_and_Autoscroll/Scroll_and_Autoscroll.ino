/*
  Scroll_and_Autoscroll.ino
  ------------------------------------------------------------
  Example for IskakINO_LiquidCrystal_I2C library

  Demonstrates:
  - scrollDisplayLeft() and scrollDisplayRight()
  - autoscroll() and noAutoscroll()
  - Text direction control (leftToRight / rightToLeft)

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

unsigned long lastAction = 0;
uint8_t stage = 0;

void setup() {
  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Scroll Demo");
  lcd.setCursor(0, 1);
  lcd.print("IskakINO LCD");
}

void loop() {
  unsigned long now = millis();

  if (now - lastAction < 1500) return;
  lastAction = now;

  switch (stage) {

    case 0:
      lcd.scrollDisplayLeft();
      break;

    case 1:
      lcd.scrollDisplayRight();
      break;

    case 2:
      lcd.clear();
      lcd.leftToRight();
      lcd.autoscroll();
      lcd.print("AutoScroll ON ");
      break;

    case 3:
      lcd.noAutoscroll();
      lcd.setCursor(0, 1);
      lcd.print("AutoScroll OFF");
      break;

    case 4:
      lcd.clear();
      lcd.rightToLeft();
      lcd.print("RTL Mode");
      break;

    default:
      stage = 0;
      lcd.clear();
      lcd.leftToRight();
      lcd.print("Restart Demo");
      return;
  }

  stage++;
}
