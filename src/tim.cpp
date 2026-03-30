#include <tim.h>

// Table of instances to allow static ISR wrappers to call the correct instance's onTimer() method
CTIMER* CTIMER::instances[4] = { nullptr, nullptr, nullptr, nullptr };

CTIMER::CTIMER(int id, uint64_t interval_us, void (*pTimerCallbackFunc)(), bool autoReload)
    : id(id), pTimerCallbackFunction(pTimerCallbackFunc)
{
    // ID → group + timer mapping
    groupId = id / 2;     // 0 oder 1
    timerId = id % 2;     // 0 oder 1

    // Storing the instance in the static table for ISR access
    instances[id] = this;

    // Get the hardware timer instance
    hwTimer = timerBegin(timerId, 80, true);  // 1 µs Ticks

    // Attach the appropriate ISR based on the timer ID
    switch (id) {
        case 0: timerAttachInterrupt(hwTimer, &CTIMER::isr0, true); break;
        case 1: timerAttachInterrupt(hwTimer, &CTIMER::isr1, true); break;
        case 2: timerAttachInterrupt(hwTimer, &CTIMER::isr2, true); break;
        case 3: timerAttachInterrupt(hwTimer, &CTIMER::isr3, true); break;
    }

    // Configure the timer alarm with the specified interval and auto-reload setting
    timerAlarmWrite(hwTimer, interval_us, autoReload);
}

void CTIMER::start() {
    timerAlarmEnable(hwTimer);
}

void CTIMER::stop() {
    timerAlarmDisable(hwTimer);
}

void CTIMER::onTimer() {
    // This method can be overridden by derived classes to implement specific behavior on timer interrupts
}

// ---------------- ISR-WRAPPER ----------------
// Static ISR wrappers that call the onTimer() method of the correct instance based on the timer ID
void IRAM_ATTR CTIMER::isr0() { if (instances[0]) instances[0]->onTimer(); }
void IRAM_ATTR CTIMER::isr1() { if (instances[1]) instances[1]->onTimer(); }
void IRAM_ATTR CTIMER::isr2() { if (instances[2]) instances[2]->onTimer(); }
void IRAM_ATTR CTIMER::isr3() { if (instances[3]) instances[3]->onTimer(); }





