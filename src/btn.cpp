#include <btn.h>

CButton::CButton(gpio_num_t pin, uint32_t debounceMs)
    : pin(pin), debounceTimeUs(debounceMs * 1000)
{
    flag = false;
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

bool CButton::getFlag()  const { return flag; }
void CButton::resetFlag() { flag = false; }

void CButton::update() {
        if (!isrTriggered) return;

        uint64_t now = esp_timer_get_time();
        bool raw = gpio_get_level(pin) == 0; // aktiver Low-Taster

        // Wenn Zustand stabil genug ist
        if (now - lastChangeUs >= debounceTimeUs) {
            if (raw != stableState) {
                stableState = raw;

                if (stableState) {
                    flag = true; // ENTprelltes Ereignis
                }
            }
            isrTriggered = false;
        }
}