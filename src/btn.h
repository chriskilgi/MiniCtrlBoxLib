#pragma once
#include <driver/gpio.h>
#include <esp_timer.h>

#define PIN_BUTTON_1 GPIO_NUM_10
#define PIN_BUTTON_2 GPIO_NUM_3

namespace nspMiniCtrlBox {
    class CButton {
    public:
        CButton(gpio_num_t interruptPin, uint32_t ui32DebounceMs= 30);

        ~CButton(); 

        bool hasButtonPressed() const;
        bool hasButtonReleased() const;
        void resetPressedFlag();
        void resetReleasedFlag();


        // Has to be called in the main loop to update the button state and set the flag
        // when a stable button press or releaseis detected
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
