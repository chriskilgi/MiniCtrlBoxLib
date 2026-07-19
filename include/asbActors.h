// asbActors.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for asbActors.cpp (Relais, Buzzer, LED)
// Neccessary for the AktorSensorBoard (ASB) of the MiniCtrlBox project.

#pragma once
#include <Arduino.h>
#include <glo.h>

#define PIN_ASB_ACTORS_RELAIS GPIO_NUM_1
#define PIN_ASB_ACTORS_BUZZER GPIO_NUM_3
#define PIN_ASB_ACTORS_LED GPIO_NUM_10

namespace nspMiniCtrlBox {
// The CASBActors class provides methods to control the actors (Relais, Buzzer, LED) connected to the AktorSensorBoard (ASB).
    class CASBActors {
        public:
            CASBActors();

            void setRelais(bool boState) {
                if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
                    digitalWrite(PIN_ASB_ACTORS_RELAIS, boState ? HIGH : LOW);
                    xSemaphoreGive(xMutexI2C_g);
                }
            }

            void setBuzzer(bool boState) {
                if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
                    digitalWrite(PIN_ASB_ACTORS_BUZZER, boState ? HIGH : LOW);
                    xSemaphoreGive(xMutexI2C_g);
                }
            }

            void setLED(bool boState) {
                if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
                    digitalWrite(PIN_ASB_ACTORS_LED, boState ? HIGH : LOW);
                    xSemaphoreGive(xMutexI2C_g);
                }
            }
        private:
    };

} // namespace nspMiniCtrlBox