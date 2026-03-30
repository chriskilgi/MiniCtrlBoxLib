#pragma once
#include <Arduino.h>

class CTIMER {
public:
    CTIMER(int id, uint64_t interval_us, bool autoReload = true);
    void start();
    void stop();

    virtual void onTimer();   // wird bei jedem Interrupt aufgerufen

    void setTimerCallbackFunction(void (*pTimerCallbackFunction)()) {
        this->TimerCallback = pTimerCallbackFunction;
    }

protected:
    void (*TimerCallback)() = nullptr; // Zeiger auf die Timer-Callback-Funktion, die im Interrupt aufgerufen wird

private:
    hw_timer_t* hwTimer;
    int id;
    int groupId;
    int timerId;

    // Instanz-Tabelle: 4 Timer → 4 Einträge
    static CTIMER* instances[4];

    // Statische ISR-Wrapper
    static void IRAM_ATTR isr0();
    static void IRAM_ATTR isr1();
    static void IRAM_ATTR isr2();
    static void IRAM_ATTR isr3();
};

class CMCBTIMER : public CTIMER {
public:
    CMCBTIMER(int id, uint64_t interval_us)
        : CTIMER(id, interval_us, true) {}

    void onTimer() override {
        if (this->TimerCallback != nullptr) {
            this->TimerCallback(); // Rufe die Callback-Funktion auf, wenn sie gesetzt ist
        }  
    }
};