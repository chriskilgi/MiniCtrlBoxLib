#include <glo.h>

#include <Wire.h>

namespace nspMiniCtrlBox {
bool gloIsI2CDevicePresent(uint8_t address) {
    Wire.beginTransmission(address);
    return (Wire.endTransmission() == 0);
}
} // namespace nspMiniCtrlBox
