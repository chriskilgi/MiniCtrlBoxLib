#include <asbActors.h>

namespace nspMiniCtrlBox {
CASBActors::CASBActors() {
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        pinMode(PIN_ASB_ACTORS_RELAIS, OUTPUT);
        pinMode(PIN_ASB_ACTORS_BUZZER, OUTPUT);
        pinMode(PIN_ASB_ACTORS_LED, OUTPUT);

        digitalWrite(PIN_ASB_ACTORS_RELAIS, LOW);
        digitalWrite(PIN_ASB_ACTORS_BUZZER, LOW);
        digitalWrite(PIN_ASB_ACTORS_LED, LOW);

        xSemaphoreGive(xMutexI2C_g);
    }
}
} // namespace nspMiniCtrlBox
