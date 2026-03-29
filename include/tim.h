#pragma once
#include <Arduino.h>

class CTIMER {
public:
    CTIMER(int id, uint64_t interval_us, bool& boGlobVar,bool autoReload = true);
    void start();
    void stop();

    virtual void onTimer();   // wird bei jedem Interrupt aufgerufen

protected:
    bool &boGlobVar; // Referenz auf die globale Variable, die im Timer-Interrupt gesetzt wird

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
    CMCBTIMER(int id, uint64_t interval_us, bool& boGlobVar)
        : CTIMER(id, interval_us, boGlobVar, true) {}

    void onTimer() override {
        this->boGlobVar = true; // Setze die globale Variable, um das Ereignis anzuzeigen
    }
};