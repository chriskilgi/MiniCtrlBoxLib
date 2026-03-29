#pragma once
#include <Arduino.h>

#define PIN_SSR 2 // GPIO pin connected to the SSR (Solid State Relay)

class CSSR {
public:
    CSSR(gpio_num_t pin = GPIO_NUM_2); // Constructor with default pin GPIO 2

    void on();
    void off();

private:
    gpio_num_t pin;
};