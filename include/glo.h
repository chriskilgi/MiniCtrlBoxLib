// glo.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for glo.cpp (Global Functions)
// Neccessary for the Mainboard of the MiniCtrlBox project.

#pragma once
#include <stdint.h>

namespace nspMiniCtrlBox {
#define MINICTRLBOX_SW_VERSION "6.1.0"
#define MINICTRLBOX_HW_VERSION "3.0.1"

// Global function to check if an I2C device is present at the specified address, with optional retries and delay between retries
bool gloIsI2CDevicePresent(uint8_t address, uint8_t retries = 5, uint16_t delayMs = 2);
} // namespace nspMiniCtrlBox