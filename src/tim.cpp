#include <tim.h>

// Instanz-Tabelle definieren
CTIMER* CTIMER::instances[4] = { nullptr, nullptr, nullptr, nullptr };

CTIMER::CTIMER(int id, uint64_t interval_us, bool autoReload)
    : id(id)
{
    // ID → group + timer umrechnen
    groupId = id / 2;     // 0 oder 1
    timerId = id % 2;     // 0 oder 1

    // Instanz registrieren
    instances[id] = this;

    // Hardware-Timer erzeugen
    hwTimer = timerBegin(timerId, 80, true);  // 1 µs Ticks

    // ISR verbinden
    switch (id) {
        case 0: timerAttachInterrupt(hwTimer, &CTIMER::isr0, true); break;
        case 1: timerAttachInterrupt(hwTimer, &CTIMER::isr1, true); break;
        case 2: timerAttachInterrupt(hwTimer, &CTIMER::isr2, true); break;
        case 3: timerAttachInterrupt(hwTimer, &CTIMER::isr3, true); break;
    }

    // Alarm konfigurieren
    timerAlarmWrite(hwTimer, interval_us, autoReload);
}

void CTIMER::start() {
    timerAlarmEnable(hwTimer);
}

void CTIMER::stop() {
    timerAlarmDisable(hwTimer);
}

void CTIMER::onTimer() {
    // Standard: nichts tun
}

// ---------------- ISR-WRAPPER ----------------

void IRAM_ATTR CTIMER::isr0() { if (instances[0]) instances[0]->onTimer(); }
void IRAM_ATTR CTIMER::isr1() { if (instances[1]) instances[1]->onTimer(); }
void IRAM_ATTR CTIMER::isr2() { if (instances[2]) instances[2]->onTimer(); }
void IRAM_ATTR CTIMER::isr3() { if (instances[3]) instances[3]->onTimer(); }





