#include <ssr.h>

CSSR::CSSR(gpio_num_t pin) : pin(pin) {
    pinMode(pin, OUTPUT);
    off(); // Ensure the SSR is off at startup
}

void CSSR::on() {
    digitalWrite(pin, HIGH);
}

void CSSR::off() {
    digitalWrite(pin, LOW);
}
