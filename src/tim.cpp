#include <tim.h>

namespace nspMiniCtrlBox {
// Table of instances to allow static ISR wrappers to call the correct instance's onTimer() method
CTimer* CTimer::instances[4] = { nullptr, nullptr, nullptr, nullptr };

CTimer::CTimer(int iHWTimer, uint64_t ui64TriggerIntervall, void (*pTimerCallbackFunc)(), bool boAutoReload)
    : iHWTimer(iHWTimer), pTimerCallbackFunction(pTimerCallbackFunc)
{
    // ID → group + timer mapping
    groupId = iHWTimer / 2;     // 0 oder 1
    timerId = iHWTimer % 2;     // 0 oder 1

    // Storing the instance in the static table for ISR access
    instances[iHWTimer] = this;

    // Get the hardware timer instance
    pHwTimer = timerBegin(timerId, 80, true);  // 1 µs Ticks

    // Attach the appropriate ISR based on the timer ID
    switch (iHWTimer) {
        case 0: timerAttachInterrupt(pHwTimer, &CTimer::isr0, true); break;
        case 1: timerAttachInterrupt(pHwTimer, &CTimer::isr1, true); break;
        case 2: timerAttachInterrupt(pHwTimer, &CTimer::isr2, true); break;
        case 3: timerAttachInterrupt(pHwTimer, &CTimer::isr3, true); break;
    }

    // Configure the timer alarm with the specified interval and auto-reload setting
    timerAlarmWrite(pHwTimer, ui64TriggerIntervall, boAutoReload);
}

void CTimer::start() {
    timerAlarmEnable(pHwTimer);
}

void CTimer::stop() {
    timerAlarmDisable(pHwTimer);
}

// ---------------- ISR-WRAPPER ----------------
// Static ISR wrappers that call the pTimerCallbackFunction() method of the correct
// instance based on the timer ID
void IRAM_ATTR CTimer::isr0() {
    if (instances[0] && instances[0]->pTimerCallbackFunction)
        instances[0]->pTimerCallbackFunction();
}
void IRAM_ATTR CTimer::isr1() { 
    if (instances[1] && instances[1]->pTimerCallbackFunction) 
        instances[1]->pTimerCallbackFunction();
}
void IRAM_ATTR CTimer::isr2() { 
    if (instances[2] && instances[2]->pTimerCallbackFunction) 
        instances[2]->pTimerCallbackFunction(); 
}
void IRAM_ATTR CTimer::isr3() { 
    if (instances[3] && instances[3]->pTimerCallbackFunction) 
        instances[3]->pTimerCallbackFunction(); 
}
} // namespace nspMiniCtrlBox





