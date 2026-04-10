#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class COLed {
public:
    COLed(TwoWire& wire = Wire, int8_t rst = -1);

    bool begin();
    bool isPresent();

    void clear();
    void clearLine(uint8_t row);

    // Optionaler Parameter size:
    // 0 = aktuelle Größe
    // 1 = klein, 2 = groß
    void printAt(uint8_t row, uint8_t col, const char* text, uint8_t size = 0);
    void printLine(uint8_t row, const char* text, uint8_t size = 0);

    void printfAt(uint8_t row, uint8_t col, const char* fmt, uint8_t size, ...);
    void printfLine(uint8_t row, const char* fmt, uint8_t size, ...);

    void setTextSize(uint8_t size);

private:
    Adafruit_SSD1306 display;
    bool boIsPresent = false;

    uint8_t textSize;

    // Dynamische Zeichenmaße
    uint8_t charW;
    uint8_t charH;
    uint8_t rows;
    uint8_t cols;

    static constexpr uint8_t WIDTH = 128;
    static constexpr uint8_t HEIGHT = 64;
    static constexpr uint8_t DISPLAY_ADDRESS = 0x3C;

    static constexpr uint8_t BASE_CHAR_W = 6;
    static constexpr uint8_t BASE_CHAR_H = 8;
};
