#pragma once
#include <Arduino.h>

// Abstraction for ESP32 hardware timers with callback functions and auto-reload functionality.
class CTimer {
public:
    CTimer(int id, uint64_t interval_us, void (*pTimerCallbackFunction)() = nullptr, bool autoReload = true);
    void start();
    void stop();

    virtual void onTimer();   // wird bei jedem Interrupt aufgerufen

    // Method to set or change the timer callback function,
    // allowing dynamic assignment of the callback after timer creation
    void setTimerCallbackFunction(void (*pTimerCallbackFunction)()) {
        this->pTimerCallbackFunction = pTimerCallbackFunction;
    }

protected:
    // Pointer to the timer callback function, which will be called in the interrupt service routine
    void (*pTimerCallbackFunction)() = nullptr; // Initially set to nullptr, can be assigned a valid function pointer by the user


private:
    hw_timer_t* hwTimer;
    int id; // Timer ID (0-3, where 0 and 1 belong to group 0, and 2 and 3 belong to group 1)
    int groupId; // Timer group ID (0 or 1)
    int timerId; // Timer ID within the group (0 or 1)

    // Table of instances to allow static ISR wrappers to call the correct instance's onTimer() method
    static CTimer* instances[4];

    // Static ISR wrappers that call the onTimer() method of the correct instance based on the timer ID
    static void IRAM_ATTR isr0();
    static void IRAM_ATTR isr1();
    static void IRAM_ATTR isr2();
    static void IRAM_ATTR isr3();
};

// Specialized timer class that calls a user-defined callback function on each timer interrupt
class CMCBTimer : public CTimer {
public:
    CMCBTimer(int id, uint64_t interval_us, void (*pTimerCallbackFunction)() = nullptr)
        : CTimer(id, interval_us, pTimerCallbackFunction, true) {}

    // Override the onTimer method to call the user-defined callback function
    void onTimer() override {
        if (this->pTimerCallbackFunction != nullptr) {
            this->pTimerCallbackFunction(); // Call the user-defined callback function if it is set
        }  
    }
};