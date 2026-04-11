#pragma once
#include <stdint.h>

namespace nspMiniCtrlBox {
#define MINICTRLBOX_SW_VERSION "5.0.4"
#define MINICTRLBOX_HW_VERSION "3.0.1"

bool gloIsI2CDevicePresent(uint8_t ui8DeviceAddress);
} // namespace nspMiniCtrlBox