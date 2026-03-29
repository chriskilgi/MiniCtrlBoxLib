#pragma once
#include <I2C_eeprom.h>
#include <glo.h>

#define EEPROM_ADRESS 0x50

struct TEEPROM {
  uint8_t ui8ID;                    // z.B. 1
  char acProjektbezeichnung[20];    // z.B. "MiniCtrlBox"
  char acBoardbezeichnung[20];      // z.B. "Mainboard"
  char acSWVersion[10];             // z.B. "1.0.0"
  char acHWVersion[10];             // z.B. "3.0.1"
};

class CEEPROM {
public:
    CEEPROM(uint8_t deviceAddress = EEPROM_ADRESS);
    bool isPresent();
    uint8_t getAddress();

    void writeDeviceInfo(const TEEPROM* pDeviceInfo);
    void readDeviceInfo(TEEPROM* pDeviceInfo);

    uint8_t getID() const { return tDeviceInfo.ui8ID; }

    const char* getProjectName()  {
        if (!boDeviceInfoLoaded) {
            // If device info is not loaded, we should read it first
            readDeviceInfo(&tDeviceInfo);
        }
        return tDeviceInfo.acProjektbezeichnung;
    }
    const char* getBoardName() {
        if (!boDeviceInfoLoaded) {
            // If device info is not loaded, we should read it first
            readDeviceInfo(&tDeviceInfo);
        }
        return tDeviceInfo.acBoardbezeichnung;
    }
    const char* getHWVersion() {
        if (!boDeviceInfoLoaded) {
            // If device info is not loaded, we should read it first
            readDeviceInfo(&tDeviceInfo);
        }
        return tDeviceInfo.acHWVersion;
    }

    const char* getSWVersion() {
        if (!boDeviceInfoLoaded) {
            // If device info is not loaded, we should read it first
            readDeviceInfo(&tDeviceInfo);
        }
        return tDeviceInfo.acSWVersion;
    }

    uint8_t getFreeSpace() {
        // Calculate free space based on the size of TEEPROM and total EEPROM size
        return I2C_DEVICESIZE_24LC02 - sizeof(TEEPROM);
    }
    void setHWVersion(const char* version);
    void setSWVersion(const char* version);

    bool writeUserData(const uint8_t * buffer, uint16_t length);
    bool readUserData(uint8_t * buffer, uint16_t length);
private:
    I2C_eeprom eeprom;
    TEEPROM tDeviceInfo; // Local copy of the device info for easy access
    bool boDeviceInfoLoaded = false; // Flag to indicate if device info has been loaded into tDeviceInfo
};
