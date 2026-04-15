// dsp.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for dsp.cpp (OLED-Display)
// Neccessary for the Mainboard of the MiniCtrlBox project.

#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>

namespace nspMiniCtrlBox {
    // The COLed class provides methods to control the OLED display (SSD1306) connected to the Mainboard of the MiniCtrlBox project.
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
