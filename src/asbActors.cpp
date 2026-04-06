#include <asbActors.h>

CASBActors::CASBActors() {
    // Initialize the pins for the actors
    pinmode(PIN_ASB_ACTORS_BUZZER, OUTPUT);
    pinmode(PIN_ASB_ACTORS_RELAIS, OUTPUT);
    pinmode(PIN_ASB_ACTORS_LED, OUTPUT);

    // Set all actors to a known state (off)
    digitalWrite(PIN_ASB_ACTORS_BUZZER, LOW);
    digitalWrite(PIN_ASB_ACTORS_RELAIS, LOW);
    digitalWrite(PIN_ASB_ACTORS_LED, LOW);
}


