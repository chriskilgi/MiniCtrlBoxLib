#pragma once
#include <I2C_eeprom.h>
#include <glo.h>

#define EEPROM_ADRESS 0x50

namespace nspMiniCtrlBox {
    struct TEEPROM {   
    uint8_t ui8ID;                    // z.B. 1
    char acProjectname[20];           // z.B. "MiniCtrlBox"
    char acBoardname[20];             // z.B. "Mainboard"
    char acSWVersion[10];             // z.B. "1.0.0"
    char acHWVersion[10];             // z.B. "3.0.1"
    };

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
