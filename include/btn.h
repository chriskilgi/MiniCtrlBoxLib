// btn.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for btn.cpp (Button)
// Neccessary for the Mainboard of the MiniCtrlBox project.

#pragma once
#include <driver/gpio.h>
#include <esp_timer.h>

#define PIN_BUTTON_1 GPIO_NUM_10
#define PIN_BUTTON_2 GPIO_NUM_3
#define PIN_BUTTON_LEFT PIN_BUTTON_2
#define PIN_BUTTON_RIGHT PIN_BUTTON_1

namespace nspMiniCtrlBox {
    // The CButton class provides debounced button handling with interrupt support for the buttons connected to the Mainboard of the MiniCtrlBox project.
    class CButton {
    public:
        CButton(gpio_num_t interruptPin, uint32_t ui32DebounceMs= 30);

        ~CButton(); 

        bool hasButtonPressed(bool boResetFlag = true);
        bool hasButtonReleased(bool boResetFlag = true);
        void resetPressedFlag();
        void resetReleasedFlag();


        // Has to be called in the main loop to update the button state and set the flag
        // when a stable button press or releaseis detected
        // 11.10.2026: the method is now called within hasButtonPressed() and hasButtonReleased() 
        // so, it is not longer necessary to call it in the main loop anymore.
        // But it does not harm to call it in the main loop, so it is still possible to do so.
        void updateFlags();

    private:
        gpio_num_t pin;
        volatile bool boPressedFlag;
        volatile bool boReleasedFlag;

        uint64_t debounceTimeUs;
        uint64_t lastChangeUs;

        volatile bool isrTriggered;
        bool stableState;

        static void IRAM_ATTR isrHandler(void* arg) {
            CButton* self = static_cast<CButton*>(arg);

            self->lastChangeUs = esp_timer_get_time();
            self->isrTriggered = true;
        }
    };
} // namespace nspMiniCtrlBox
