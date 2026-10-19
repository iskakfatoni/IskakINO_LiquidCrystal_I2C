/*
 * Project: IskakINO_LiquidCrystal_I2C
 * Folder: examples/06_I2CScannerMode
 * Description: Using the built-in scanner to identify I2C devices on the bus.
 * Author: Iskak Fatoni
 */

#include <IskakINO_LiquidCrystal_I2C.h>

// Inisialisasi LCD (ukuran 16x2, alamat I2C auto-detect)
LiquidCrystal_I2C lcd(16, 2);

void setup() {
  Serial.begin(9600);
  Serial.println(F("[I2C Scanner] Memulai pencarian LCD..."));

  // 1. Jalankan begin(). Library akan otomatis scan alamat I2C (0x20..0x27 & 0x38..0x3F)
  bool found = lcd.begin();

  if (found) {
    uint8_t addr = lcd.getAddress();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LCD Terdeteksi!");
    lcd.setCursor(0, 1);
    lcd.print("Addr: 0x");
    if (addr < 0x10) lcd.print("0");
    lcd.print(addr, HEX);

    Serial.print(F("[I2C Scanner] LCD ditemukan di alamat: 0x"));
    Serial.println(addr, HEX);
  } else {
    // Jika perangkat tidak ditemukan, LCD fisik tidak bisa menampilkan teks.
    // Oleh karena itu, kita laporkan status error melalui Serial Monitor.
    Serial.println(F("[I2C Scanner] Error: LCD tidak ditemukan di bus I2C!"));
    Serial.println(F("Periksa sambungan kabel SDA, SCL, VCC, dan GND."));
  }
}

void loop() {
  // Diam di sini setelah proses pemindaian selesai
  delay(1000);
}
