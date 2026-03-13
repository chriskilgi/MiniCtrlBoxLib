#pragma once
#include <driver/gpio.h>
#include <esp_timer.h>

#define PIN_BUTTON_1 GPIO_NUM_10
#define PIN_BUTTON_2 GPIO_NUM_3

class CBUTTON {
public:
    CBUTTON(gpio_num_t pin, uint32_t debounceMs = 60);

    ~CBUTTON(); 

    bool getFlag() const;
    void resetFlag();


    // Muss im loop() aufgerufen werden
    void update();

private:
    gpio_num_t pin;
    volatile bool flag;

    uint64_t debounceTimeUs;
    uint64_t lastChangeUs;

    volatile bool isrTriggered;
    bool stableState;

    static void IRAM_ATTR isrHandler(void* arg) {
        CBUTTON* self = static_cast<CBUTTON*>(arg);
        self->lastChangeUs = esp_timer_get_time();
        self->isrTriggered = true;
    }
};
