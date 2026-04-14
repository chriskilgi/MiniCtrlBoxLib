#include <glo.h>

#include <Wire.h>

namespace nspMiniCtrlBox {
bool gloIsI2CDevicePresent(uint8_t address, uint8_t retries, uint16_t delayMs) {
    for (uint8_t i = 0; i < retries; i++) {
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            return true;    // Device antwortet mit ACK
        }

        delay(delayMs);     // Warten (wichtig für EEPROMs)
    }

    return false;           // Nach mehreren Versuchen kein ACK
}

} // namespace nspMiniCtrlBox
