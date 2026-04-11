#include <glo.h>

#include <Wire.h>

namespace nspMiniCtrlBox {
bool gloIsI2CDevicePresent(uint8_t ui8DeviceAddress) {
    Wire.beginTransmission(ui8DeviceAddress);
    return (Wire.endTransmission() == 0);
}
} // namespace nspMiniCtrlBox
