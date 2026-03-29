#pragma once
#include <Adafruit_SSD1306.h>

#define DISPLAY_ADDRESS 0x3C

class COLED {
public:
    static constexpr uint8_t WIDTH  = 128;
    static constexpr uint8_t HEIGHT = 64;
    static constexpr uint8_t CHAR_W = 6;   // Standardfont
    static constexpr uint8_t CHAR_H = 8;
    static constexpr uint8_t COLS   = WIDTH / CHAR_W;   // 21
    static constexpr uint8_t ROWS   = HEIGHT / CHAR_H;  // 8

    COLED(TwoWire& wire = Wire, int8_t rst = -1);

    bool begin();

    // Zeile löschen
    void clearLine(uint8_t row);

    // Text an Zeile/Spalte ausgeben
    void printAt(uint8_t row, uint8_t col, const char* text);

    // Ganze Zeile überschreiben (automatisch)
    void printLine(uint8_t row, const char* text);

    // Komplettes Display löschen
    void clear();

private:
    Adafruit_SSD1306 display;
};
