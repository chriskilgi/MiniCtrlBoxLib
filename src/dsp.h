#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>

namespace nspMiniCtrlBox {
    class COLed {
    public:
        COLed(TwoWire& wire = Wire, int8_t rst = -1);

        bool begin();
        bool isPresent();

        void clear();
        void clearLine(uint8_t ui8Row);

        void printAt(uint8_t ui8Row, uint8_t ui8Col, const char* pcText);
        void printLine(uint8_t ui8Row, const char* pcText);

        void printfAt(uint8_t ui8Row, uint8_t ui8Col, const char* pcFormattedText, ...);
        void printfLine(uint8_t ui8Row, const char* pcFormattedText, ...);

        void setTextSize(uint8_t ui8Size);

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
} // namespace nspMiniCtrlBox
