#include <asbActors.h>

namespace nspMiniCtrlBox {
CASBActors::CASBActors() {
    pinMode(PIN_ASB_ACTORS_RELAIS, OUTPUT);
    pinMode(PIN_ASB_ACTORS_BUZZER, OUTPUT);
    pinMode(PIN_ASB_ACTORS_LED, OUTPUT);

    digitalWrite(PIN_ASB_ACTORS_RELAIS, LOW);
    digitalWrite(PIN_ASB_ACTORS_BUZZER, LOW);
    digitalWrite(PIN_ASB_ACTORS_LED, LOW);
}
} // namespace nspMiniCtrlBox
