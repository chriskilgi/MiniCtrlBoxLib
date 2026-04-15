// eep.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for eep.cpp (EEPROM)
// Neccessary for the Mainboard of the MiniCtrlBox project.

#pragma once
#include <I2C_eeprom.h>
#include <glo.h>

#define EEPROM_ADRESS 0x50

namespace nspMiniCtrlBox {
    // Struct to hold the device information that will be stored in the EEPROM
    struct TEEPROM {   
    uint8_t ui8ID;                    // z.B. 1
    char acProjectname[20];           // z.B. "MiniCtrlBox"
    char acBoardname[20];             // z.B. "Mainboard"
    char acSWVersion[10];             // z.B. "1.0.0"
    char acHWVersion[10];             // z.B. "3.0.1"
    };

    // The CEeprom class provides methods to interact with the EEPROM connected to the Mainboard of the MiniCtrlBox project.
    // The class can also be used for EEPROMS of additional boards like the SwitchLEDBoard, but then the device address has to be specified in the constructor.
    // The first part of the EEPROM is reserved for the TEEPROM struct which holds the device information. The rest of the EEPROM can be used for user data.
    class CEeprom {
    public:
        CEeprom(uint8_t deviceAddress = EEPROM_ADRESS);
        bool begin() {
            Wire.begin(); // Initialize I2C communication
            return eeprom.begin();
        }
        bool isPresent(uint8_t ui8DeviceAddress = EEPROM_ADRESS);
        uint8_t getAddress();

        void writeDeviceInfo(const TEEPROM* pDeviceInfo);
        void readDeviceInfo(TEEPROM* pDeviceInfo);

        // Function to get the free space in the EEPROM for user data, based on the size of the TEEPROM struct and the total EEPROM size
        uint8_t getFreeSpace() {
            // Calculate free space based on the size of TEEPROM and total EEPROM size
            return I2C_DEVICESIZE_24LC02 - sizeof(TEEPROM);
        }
        void writeID(uint8_t ui8ID);
        void writeProjectName(const char* pcName);
        void writeBoardName(const char* pcName);
        void writeHWVersion(const char* pcVersion);
        void writeSWVersion(const char* pcVersion);
        uint8_t readID();
        const char* readHWVersion();
        const char* readSWVersion();
        const char* readProjectName();
        const char* readBoardName();

        bool writeUserData(const void* pvBuffer, uint16_t ui16Length);
        bool readUserData(const void* pvBuffer, uint16_t ui16Length);
    private:
        I2C_eeprom eeprom;
        TEEPROM tDeviceInfo; // Local copy of the device info for easy access
        bool boDeviceInfoLoaded = false; // Flag to indicate if device info has been loaded into tDeviceInfo
    };
} // namespace nspMiniCtrlBox
