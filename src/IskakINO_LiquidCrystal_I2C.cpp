#include "IskakINO_LiquidCrystal_I2C.h"
#include <Arduino.h>
#include <Wire.h>

// Definisi static member
bool LiquidCrystal_I2C::_wireInitialized = false;

// Dipanggil user setelah Wire.begin(SDA, SCL) manual (ESP32/ESP8266)
void LiquidCrystal_I2C::useExternalWireBegin() {
    _wireInitialized = true;
}

/* =========================================================
   Constructors
========================================================= */
LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t cols, uint8_t rows)
    : _cols(cols),
      _rows(rows),
      _addr(0x00),
      _backlight(true),
      _initialized(false),
      _displayfunction(LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS),
      _displaycontrol(0),
      _displaymode(0),
      _wire(&Wire),
#ifdef ISKAKINO_NO_SPLASH
      _showSplash(false),
#else
      _showSplash(true),
#endif
      _twIndex(0),
      _twRow(0),
      _twIntervalMs(100),
      _twLastMillis(0),
      _twActive(false),
      _twIsScrollMode(false),
      _backlightTimeoutMs(0),
      _lastActivityMillis(0),
      _progressBarReady(false)
{
}

// Konstruktor standar 3-parameter untuk kompatibilitas penuh ekosistem Arduino
LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t addr, uint8_t cols, uint8_t rows)
    : _cols(cols),
      _rows(rows),
      _addr(addr),
      _backlight(true),
      _initialized(false),
      _displayfunction(LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS),
      _displaycontrol(0),
      _displaymode(0),
      _wire(&Wire),
#ifdef ISKAKINO_NO_SPLASH
      _showSplash(false),
#else
      _showSplash(true),
#endif
      _twIndex(0),
      _twRow(0),
      _twIntervalMs(100),
      _twLastMillis(0),
      _twActive(false),
      _twIsScrollMode(false),
      _backlightTimeoutMs(0),
      _lastActivityMillis(0),
      _progressBarReady(false)
{
}

/* =========================================================
   Bus & Config Controls
========================================================= */
void LiquidCrystal_I2C::setWire(TwoWire* wire) {
    if (wire != nullptr) {
        _wire = wire;
    }
}

void LiquidCrystal_I2C::enableSplashScreen(bool enable) {
    _showSplash = enable;
}

/* =========================================================
   Address control
========================================================= */
void LiquidCrystal_I2C::setAddress(uint8_t addr) {
    _addr = addr;

#if LCD_ENABLE_SERIAL_DEBUG
    Serial.print(F("[LCD] Manual I2C address set: 0x"));
    Serial.println(_addr, HEX);
#endif

    if (_initialized) {
        _initialized = false;
        begin();
    }
}

uint8_t LiquidCrystal_I2C::getAddress() const {
    return _addr;
}

/* =========================================================
   Custom character
========================================================= */
void LiquidCrystal_I2C::createChar(uint8_t location, const uint8_t charmap[]) {
    location &= 0x7;
    _command(LCD_SETCGRAMADDR | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}

void LiquidCrystal_I2C::createChar_P(uint8_t location, const uint8_t charmap[]) {
    location &= 0x7;
    _command(LCD_SETCGRAMADDR | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        write(pgm_read_byte(&charmap[i]));
    }
}

/* =========================================================
   Init
========================================================= */
bool LiquidCrystal_I2C::begin() {
    if (_initialized) return true;

    if (!_wireInitialized) {
        _wire->begin();
        _wire->setClock(100000);
        _wireInitialized = true;
    }

    _scanAddress();
    if (_addr == 0x00) return false;

    if (_rows > 1) {
        _displayfunction |= LCD_2LINE;
    }

    delay(50);
    _write4bits(0x03 << 4);
    delayMicroseconds(4500);
    _write4bits(0x03 << 4);
    delayMicroseconds(4500);
    _write4bits(0x03 << 4);
    delayMicroseconds(150);
    _write4bits(0x02 << 4);

    _command(LCD_FUNCTIONSET | _displayfunction);

    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();
    clear();

    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    _command(LCD_ENTRYMODESET | _displaymode);
    home();

    _initialized = true;
    _lastActivityMillis = millis();

    if (_showSplash) {
        _showSplashScreen();
    }

    return true;
}

void LiquidCrystal_I2C::clear() {
    _command(LCD_CLEARDISPLAY);
    delayMicroseconds(2000);
}

void LiquidCrystal_I2C::home() {
    _command(LCD_RETURNHOME);
    delayMicroseconds(2000);
}

void LiquidCrystal_I2C::setCursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row >= _rows) row = _rows - 1;
    _command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LiquidCrystal_I2C::display() {
    _displaycontrol |= LCD_DISPLAYON;
    _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::cursor() {
    _displaycontrol |= LCD_CURSORON;
    _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::blink() {
    _displaycontrol |= LCD_BLINKON;
    _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::backlight() {
    _backlight = true;
    _lastActivityMillis = millis();
    _expanderWrite(0);
}

void LiquidCrystal_I2C::noBacklight() {
    _backlight = false;
    _expanderWrite(0);
}

void LiquidCrystal_I2C::setBacklightTimeout(unsigned long timeoutMs) {
    _backlightTimeoutMs = timeoutMs;
    _lastActivityMillis = millis();
}

void LiquidCrystal_I2C::scrollDisplayLeft() {
    _command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LiquidCrystal_I2C::scrollDisplayRight() {
    _command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LiquidCrystal_I2C::leftToRight() {
    _displaymode |= LCD_ENTRYLEFT;
    _command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::rightToLeft() {
    _displaymode &= ~LCD_ENTRYLEFT;
    _command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::autoscroll() {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    _command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::noAutoscroll() {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    _command(LCD_ENTRYMODESET | _displaymode);
}

size_t LiquidCrystal_I2C::write(uint8_t value) {
    _lastActivityMillis = millis();
    _send(value, LCD_RS);
    return 1;
}

/* =========================================================
   Low-level command handling
========================================================= */
void LiquidCrystal_I2C::_command(uint8_t value) {
    _send(value, 0);
}

void LiquidCrystal_I2C::_send(uint8_t value, uint8_t mode) {
    if (_addr == 0x00) return;

    uint8_t highnib = value & 0xF0;
    uint8_t lownib = (value << 4) & 0xF0;
    _write4bits(highnib | mode);
    _write4bits(lownib | mode);
}

// Dioptimalkan: menghapus expanderWrite redundan sebelum pulseEnable
void LiquidCrystal_I2C::_write4bits(uint8_t value) {
    _pulseEnable(value);
}

void LiquidCrystal_I2C::_pulseEnable(uint8_t data) {
    _expanderWrite(data | LCD_EN);
    delayMicroseconds(1);
    _expanderWrite(data & ~LCD_EN);
    delayMicroseconds(50);
}

void LiquidCrystal_I2C::_expanderWrite(uint8_t data) {
    if (_addr == 0x00) return;

    _wire->beginTransmission(_addr);
    _wire->write(data | (_backlight ? LCD_BACKLIGHT : LCD_NOBACKLIGHT));
    _wire->endTransmission();
}

/* =========================================================
   I2C Address Auto Scan
========================================================= */
void LiquidCrystal_I2C::_scanAddress() {
    if (_addr != 0x00) return;

    for (uint8_t addr = 0x20; addr <= 0x27; addr++) {
        _wire->beginTransmission(addr);
        if (_wire->endTransmission() == 0) {
            _addr = addr;
#if LCD_ENABLE_SERIAL_DEBUG
            Serial.print(F("[LCD] Found at 0x"));
            Serial.println(_addr, HEX);
#endif
            return;
        }
    }

    for (uint8_t addr = 0x38; addr <= 0x3F; addr++) {
        _wire->beginTransmission(addr);
        if (_wire->endTransmission() == 0) {
            _addr = addr;
#if LCD_ENABLE_SERIAL_DEBUG
            Serial.print(F("[LCD] Found at 0x"));
            Serial.println(_addr, HEX);
#endif
            return;
        }
    }

#if LCD_ENABLE_SERIAL_DEBUG
    Serial.println(F("[LCD] I2C device NOT found"));
#endif
}

/* =========================================================
   printCenter — aman dari overflow buffer/baris
========================================================= */
void LiquidCrystal_I2C::_printCenterImpl(const char* text, int row) {
    int len = strlen(text);
    int pos = (_cols - len) / 2;
    if (pos < 0) pos = 0;

    setCursor(pos, row);
    for (int i = 0; i < (_cols - pos) && text[i] != '\0'; i++) {
        print(text[i]);
    }
}

void LiquidCrystal_I2C::printCenter(const char* text, int row) {
    _printCenterImpl(text, row);
}

void LiquidCrystal_I2C::printCenter(const String& text, int row) {
    _printCenterImpl(text.c_str(), row);
}

/* =========================================================
   typewriter() versi BLOCKING
========================================================= */
void LiquidCrystal_I2C::_typewriterBlockingImpl(const char* text, int row, int delayTime) {
    setCursor(0, row);
    size_t len = strlen(text);
    for (size_t i = 0; i < len; i++) {
        if (i < (size_t)_cols) {
            print(text[i]);
            delay(delayTime);
        }
    }
}

void LiquidCrystal_I2C::typewriter(const char* text, int row, int delayTime) {
    _typewriterBlockingImpl(text, row, delayTime);
}

void LiquidCrystal_I2C::typewriter(const String& text, int row, int delayTime) {
    _typewriterBlockingImpl(text.c_str(), row, delayTime);
}

/* =========================================================
   typewriter NON-BLOCKING
========================================================= */
void LiquidCrystal_I2C::typewriterStart(const char* text, int row, int delayTime) {
    _twBuffer = text;
    _twIndex = 0;
    _twRow = (row < _rows) ? row : (_rows - 1);
    _twIntervalMs = (uint16_t)delayTime;
    _twLastMillis = millis();
    _twActive = true;
    _twIsScrollMode = false;

    setCursor(0, _twRow);
    for (uint8_t i = 0; i < _cols; i++) print(' ');
    setCursor(0, _twRow);
}

void LiquidCrystal_I2C::typewriterStart(const String& text, int row, int delayTime) {
    typewriterStart(text.c_str(), row, delayTime);
}

void LiquidCrystal_I2C::typewriterStop() {
    _twActive = false;
}

bool LiquidCrystal_I2C::isTypewriterActive() const {
    return _twActive && !_twIsScrollMode;
}

/* =========================================================
   Auto horizontal scroll text (NON-BLOCKING)
========================================================= */
void LiquidCrystal_I2C::scrollTextStart(const char* text, int row, uint16_t intervalMs) {
    _twBuffer = text;
    // Jika teks lebih pendek dari lebar layar, pad spasi agar mulus dan tidak menduplikasi
    if (_twBuffer.length() < _cols) {
        while (_twBuffer.length() < _cols) {
            _twBuffer += ' ';
        }
    }
    _twBuffer += "   "; // Spasi pemisah antar perulangan

    _twIndex = 0;
    _twRow = (row < _rows) ? row : (_rows - 1);
    _twIntervalMs = intervalMs;
    _twLastMillis = millis();
    _twActive = true;
    _twIsScrollMode = true;
}

void LiquidCrystal_I2C::scrollTextStart(const String& text, int row, uint16_t intervalMs) {
    scrollTextStart(text.c_str(), row, intervalMs);
}

void LiquidCrystal_I2C::scrollTextStop() {
    _twActive = false;
}

bool LiquidCrystal_I2C::isScrollActive() const {
    return _twActive && _twIsScrollMode;
}

/* =========================================================
   update() — dipanggil tiap loop()
========================================================= */
void LiquidCrystal_I2C::update() {
    unsigned long now = millis();

    // --- Backlight auto-timeout ---
    if (_backlightTimeoutMs > 0 && _backlight) {
        if (now - _lastActivityMillis >= _backlightTimeoutMs) {
            noBacklight();
        }
    }

    // --- Typewriter / Scroll ---
    if (!_twActive) return;
    if (now - _twLastMillis < _twIntervalMs) return;
    _twLastMillis = now;

    if (!_twIsScrollMode) {
        if (_twIndex >= _twBuffer.length() || _twIndex >= _cols) {
            _twActive = false;
            return;
        }
        setCursor(_twIndex, _twRow);
        print(_twBuffer[_twIndex]);
        _twIndex++;
    } else {
        uint16_t totalLen = _twBuffer.length();
        if (totalLen == 0) {
            _twActive = false;
            return;
        }

        setCursor(0, _twRow);
        for (uint8_t i = 0; i < _cols; i++) {
            uint16_t charPos = (_twIndex + i) % totalLen;
            print(_twBuffer[charPos]);
        }

        _twIndex++;
        if (_twIndex >= totalLen) _twIndex = 0;
    }
}

/* =========================================================
   Progress bar built-in
========================================================= */
void LiquidCrystal_I2C::drawProgressBar(uint8_t percent, uint8_t row) {
    if (percent > 100) percent = 100;

    if (!_progressBarReady) {
        static const uint8_t block[8] = {
            0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F
        };
        createChar(LCD_PROGRESSBAR_CHAR_LOC, block);
        _progressBarReady = true;
    }

    uint8_t filled = (uint16_t)percent * _cols / 100;

    setCursor(0, row);
    for (uint8_t i = 0; i < _cols; i++) {
        write(i < filled ? (uint8_t)LCD_PROGRESSBAR_CHAR_LOC : (uint8_t)' ');
    }
}

/* =========================================================
   printf-style formatted print
========================================================= */
void LiquidCrystal_I2C::printFormatted(const char* format, ...) {
    char buffer[LCD_PRINTF_BUFFER_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    print(buffer);
}

/* =========================================================
   isConnected()
========================================================= */
bool LiquidCrystal_I2C::isConnected() {
    if (_addr == 0x00) return false;
    _wire->beginTransmission(_addr);
    return (_wire->endTransmission() == 0);
}

void LiquidCrystal_I2C::_showSplashScreen() {
    backlight();
    clear();

    setCursor(0, 0);
    print("@iskakfatoni");

    if (_rows > 1) {
        setCursor(0, 1);
        print("I2C Addr: 0x");
        if (_addr < 0x10) print("0");
        print(_addr, HEX);
    }

    delay(2000);
    clear();
    home();
}
