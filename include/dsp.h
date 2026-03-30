#pragma once
#include <Adafruit_SSD1306.h>

#define DISPLAY_ADDRESS 0x3C

class COLed {
public:
    static constexpr uint8_t WIDTH  = 128;
    static constexpr uint8_t HEIGHT = 64;
    static constexpr uint8_t CHAR_W = 6;   // Standardfont
    static constexpr uint8_t CHAR_H = 8;
    static constexpr uint8_t COLS   = WIDTH / CHAR_W;   // 21
    static constexpr uint8_t ROWS   = HEIGHT / CHAR_H;  // 8

    COLed(TwoWire& wire = Wire, int8_t rst = -1);

    // Initializes the display and returns true if successful, false otherwise
    bool begin();

    bool isPresent();

    // Writes text at a specific row and column (starting at 0)
    void printAt(uint8_t row, uint8_t col, const char* text);

    // Override whole line (automatically starts at column 0)
    void printLine(uint8_t row, const char* text);

    // Deletes the entire display content
    void clear();

    // Deletes the content of a specific line (starting at 0)
    void clearLine(uint8_t row);

    // Prints formatted text to a specific position on the display
    // Example usage: printfAt(2, 5, "Temp: %.1f C", temperature);
    void printfAt(uint8_t row, uint8_t col, const char* fmt, ...);
    // Prints formatted text to a specific line on the display (starting at column 0)
    // Example usage: printfLine(2, "Temp: %.1f C", temperature);
    void printfLine(uint8_t row, const char* fmt, ...);

    uint8_t getRows() const { return ROWS; }
    uint8_t getCols() const { return COLS; }

private:
    Adafruit_SSD1306 display;
    bool boIsPresent;
};
