#include <I2C_eeprom.h>

#define EEPROM_ADRESS 0x50

struct TEEPROM {
  uint8_t ui8ID;
  char acProjektbezeichnung[20]; // z.B. "MiniCtrlBox"
  char acBoardbezeichnung[20]; // z.B. "Mainboard"
  char acSWVersion[10];  // z.B. "1.0.0"
  char acHWVersion[20];  // z.B. "V3.0"
};

class CEEPROM {
public:
    CEEPROM();
    bool isConnected();
    uint8_t getAddress();

    void writeDeviceInfo(const TEEPROM& deviceInfo);
    void readDeviceInfo(TEEPROM& deviceInfo);

    uint8_t getID() const { return tDeviceInfo.ui8ID; }
    const char* getProjectName()  {
        if (!boDeviceInfoLoaded) {
            // If device info is not loaded, we should read it first
            readDeviceInfo(tDeviceInfo);
        }
        return tDeviceInfo.acProjektbezeichnung;
    }
    const char* getBoardName() {
        if (!boDeviceInfoLoaded) {
            // If device info is not loaded, we should read it first
            readDeviceInfo(tDeviceInfo);
        }
        return tDeviceInfo.acBoardbezeichnung;
    }
    const char* getHWVersion() {
        if (!boDeviceInfoLoaded) {
            // If device info is not loaded, we should read it first
            readDeviceInfo(tDeviceInfo);
        }
        return tDeviceInfo.acHWVersion;
    }

    const char* getSWVersion() {
        if (!boDeviceInfoLoaded) {
            // If device info is not loaded, we should read it first
            readDeviceInfo(tDeviceInfo);
        }
        return tDeviceInfo.acSWVersion;
    }

    void setHWVersion(const char* version);
    void setSWVersion(const char* version);

private:
    void writeBlock(uint16_t memoryAddress, const uint8_t * buffer, uint16_t length);
    void readBlock(uint16_t memoryAddress, uint8_t * buffer, uint16_t length);

    I2C_eeprom eeprom;
    TEEPROM tDeviceInfo; // Local copy of the device info for easy access
    bool boDeviceInfoLoaded = false; // Flag to indicate if device info has been loaded into tDeviceInfo
};
