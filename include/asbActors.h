#pragma once
#include <Arduino.h>

#define PIN_ASB_ACTORS_RELAIS GPIO_NUM_1
#define PIN_ASB_ACTORS_BUZZER GPIO_NUM_3
#define PIN_ASB_ACTORS_LED GPIO_NUM_10

class CASBActors {
    public:
        CASBActors();

        void setRelais(bool boState) {
            digitalWrite(PIN_ASB_ACTORS_RELAIS, boState ? HIGH : LOW);
        }

        void setBuzzer(bool boState) {
            digitalWrite(PIN_ASB_ACTORS_BUZZER, boState ? HIGH : LOW);
        }

        void setLED(bool boState) {
            digitalWrite(PIN_ASB_ACTORS_LED, boState ? HIGH : LOW);
        }
    private:
};