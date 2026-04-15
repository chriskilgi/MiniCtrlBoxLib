// tim.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for tim.cpp (Relais, Buzzer, LED)
// Neccessary for the Mainboard of the MiniCtrlBox project.

#pragma once
#include <Arduino.h>

namespace nspMiniCtrlBox {
    // The CTimer class provides an interface for using the hardware timers of the ESP32 in the MiniCtrlBox project.
    // It allows setting up a timer with a specified interval and a callback function that will be
    // called when the timer expires. The timer can be set to auto-reload, meaning it will automatically restart after expiring.
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
} // namespace nspMiniCtrlBox
