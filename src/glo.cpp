#include <glo.h>

#include <Wire.h>

bool gloIsI2CDevicePresent(uint8_t address) {
    Wire.beginTransmission(address);
    return (Wire.endTransmission() == 0);
}
