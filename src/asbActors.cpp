#include <asbActors.h>

namespace nspMiniCtrlBox {
CASBActors::CASBActors() {
    // Initialize the pins for the actors
    pinMode(PIN_ASB_ACTORS_BUZZER, OUTPUT);
    pinMode(PIN_ASB_ACTORS_RELAIS, OUTPUT);
    pinMode(PIN_ASB_ACTORS_LED, OUTPUT);

    // Set all actors to a known state (off)
    digitalWrite(PIN_ASB_ACTORS_BUZZER, LOW);
    digitalWrite(PIN_ASB_ACTORS_RELAIS, LOW);
    digitalWrite(PIN_ASB_ACTORS_LED, LOW);
}
} // namespace nspMiniCtrlBox
