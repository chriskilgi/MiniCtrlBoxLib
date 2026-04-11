#include <btn.h>

namespace nspMiniCtrlBox {
CButton::CButton(gpio_num_t interruptPin, uint32_t ui32DebounceMs)
    : pin(interruptPin), debounceTimeUs(ui32DebounceMs * 1000)
{
    boPressedFlag = false;
    boReleasedFlag = false;

    isrTriggered = false;
    stableState = false;
    lastChangeUs = 0;

    gpio_config_t cfg = {};
    cfg.pin_bit_mask = (1ULL << pin);
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pull_up_en = GPIO_PULLUP_ENABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_ANYEDGE;

    gpio_config(&cfg);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin, &CButton::isrHandler, this);
}

CButton::~CButton() {
    gpio_isr_handler_remove(pin);
}

bool CButton::hasButtonPressed()  const { return boPressedFlag; }
bool CButton::hasButtonReleased() const { return boReleasedFlag; }
void CButton::resetPressedFlag() { boPressedFlag = false; }
void CButton::resetReleasedFlag() { boReleasedFlag = false; }

// This method has to be called in the main loop to update the button state
// and set the flag when a stable button press or release is detected
void CButton::updateFlags() {
    if (!isrTriggered) return; // If the ISR hasn't been triggered, there's no need to check the state

    uint64_t now = esp_timer_get_time();
    bool raw = gpio_get_level(pin) == 0; // aktiver Low-Taster

    // if the debounce time has passed since the last change, we can consider the state stable
    if (now - lastChangeUs >= debounceTimeUs) {
        // If the raw state is different from the last stable state, we have a new stable state
        if (raw != stableState) {
            stableState = raw; // store new stable state

            if (stableState == true) {
                boPressedFlag = true; // debounced press event
            } else { // stableState == false
                boReleasedFlag = true; // debounced release event
            }
        }
        isrTriggered = false; // reset the ISR trigger flag
    }
}
} // namespace nspMiniCtrlBox