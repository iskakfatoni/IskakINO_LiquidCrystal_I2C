# Changelog
Semua perubahan penting pada proyek ini akan didokumentasikan dalam file ini.
Format berdasarkan [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
dan proyek ini mengikuti [Semantic Versioning](https://semver.org/).

---

## [1.0.3] - 2026-07-08
### Diperbaiki (Fixed)
- `setAddress()` sekarang bisa dipanggil kapan saja, termasuk **setelah** `begin()`
  dijalankan. Sebelumnya, memanggil `setAddress()` setelah `begin()` tidak berefek
  karena guard `if (_initialized) return;` di dalam `begin()` mencegah re-inisialisasi.
  Sekarang, `setAddress()` otomatis mereset status internal dan memanggil ulang
  `begin()` dengan alamat I2C yang baru.
### Diubah (Changed)
- Menambahkan komentar penjelas pada `backlight()` / `noBacklight()` agar lebih
  eksplisit menjelaskan bagaimana bit backlight ditambahkan otomatis oleh
  `_expanderWrite()`. Tidak ada perubahan perilaku fungsional.

---

## [1.0.0] - 2026-02-10
### Ditambahkan (Added)
- Rilis stabil pertama dari IskakINO_LiquidCrystal_I2C
- Deteksi alamat I2C otomatis untuk PCF8574 (0x20–0x27)
- API yang kompatibel dengan library LiquidCrystal_I2C umum lainnya (backward compatible)
- Dukungan kompatibilitas untuk ESP32
- Opsi konfigurasi saat kompilasi (compile-time configuration)
- Dukungan debug serial opsional
- Contoh sketch dasar beserta referensi pengkabelan (wiring)
- Menambahkan API lengkap yang kompatibel dengan LiquidCrystal
- Menambahkan dukungan untuk cursor, blink, scroll, dan autoscroll
- Menambahkan makro perintah HD44780 yang sebelumnya belum ada
### Diubah (Changed)
- Refactor internal untuk penggunaan RAM dan flash yang lebih rendah
- Menyederhanakan alur inisialisasi tanpa mengubah perilaku lama (legacy behavior)
### Diperbaiki (Fixed)
- Peningkatan stabilitas inisialisasi I2C
- Urutan inisialisasi LCD 4-bit yang lebih andal
- Rilis stabil pertama dari IskakINO_LiquidCrystal_I2C

---
