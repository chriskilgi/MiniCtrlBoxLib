#pragma once
#include <Arduino.h>

// Abstraction for ESP32 hardware timers with callback functions and auto-reload functionality.
class CTimer {
public:
    CTimer(int iHWTimer, uint64_t ui64TriggerIntervall, void (*pTimerCallbackFunction)() = nullptr, bool boAutoReload = true);
    void start();
    void stop();

    // Method to set or change the timer callback function,
    // allowing dynamic assignment of the callback after timer creation
    void setTimerCallbackFunction(void (*pTimerCallbackFunction)()) {
        this->pTimerCallbackFunction = pTimerCallbackFunction;
    }

private:
    void (*pTimerCallbackFunction)(); // Pointer to the timer callback function
    hw_timer_t* pHwTimer;
    int iHWTimer; // Timer ID (0-3, where 0 and 1 belong to group 0, and 2 and 3 belong to group 1)
    int groupId; // Timer group ID (0 or 1)
    int timerId; // Timer ID within the group (0 or 1)

    // Table of instances to allow static ISR wrappers to call the correct instance's onTimer() method
    static CTimer* instances[4];

    // Static ISR wrappers that call the pTimerCallbackFunction() method
    // of the correct instance based on the timer ID
    static void IRAM_ATTR isr0();
    static void IRAM_ATTR isr1();
    static void IRAM_ATTR isr2();
    static void IRAM_ATTR isr3();
};
