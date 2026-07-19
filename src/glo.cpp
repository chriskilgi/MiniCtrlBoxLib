#include <glo.h>

#include <Wire.h>

namespace nspMiniCtrlBox {

SemaphoreHandle_t xMutexI2C_g = xSemaphoreCreateMutex(); // Mutex for I2C bus access

bool gloIsI2CDevicePresent(uint8_t address, uint8_t retries, uint16_t delayMs) {   
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        for (uint8_t i = 0; i < retries; i++) {
            Wire.beginTransmission(address);
            uint8_t error = Wire.endTransmission();

            if (error == 0) {
                xSemaphoreGive(xMutexI2C_g);
                return true;    // Device antwortet mit ACK
            }
            delay(delayMs);     // Warten (wichtig für EEPROMs)
        }
        xSemaphoreGive(xMutexI2C_g);
    }
    return false;           // Nach mehreren Versuchen kein ACK
}

} // namespace nspMiniCtrlBox
