#include <eep.h>

namespace nspMiniCtrlBox {
CEeprom::CEeprom(uint8_t deviceAddress) : eeprom(deviceAddress, I2C_DEVICESIZE_24LC02)
{

}

bool CEeprom::isPresent()
{
    return eeprom.isConnected();
}

uint8_t CEeprom::getAddress()
{
    return eeprom.getAddress();
}

void CEeprom::writeDeviceInfo(const TEEPROM* pDeviceInfo) {
    tDeviceInfo = *pDeviceInfo;
    eeprom.writeBlock(0, (const uint8_t*)pDeviceInfo, sizeof(TEEPROM));
}

void CEeprom::readDeviceInfo(TEEPROM* pDeviceInfo) {
    eeprom.readBlock(0, (uint8_t*)pDeviceInfo, sizeof(TEEPROM));
    tDeviceInfo = *pDeviceInfo; // Update local copy
    boDeviceInfoLoaded = true;
}

void CEeprom::writeID(uint8_t ui8ID) {
    if (isPresent()) {
        eeprom.writeBlock(offsetof(TEEPROM, ui8ID), (const uint8_t*)&ui8ID, sizeof(TEEPROM::ui8ID)); // Write only the ID field to EEPROM
    }
}

uint8_t CEeprom::readID() {
    uint8_t ui8ID;
    if (isPresent()) {
        // Read only the ID field from EEPROM
        eeprom.readBlock(offsetof(TEEPROM, ui8ID), (uint8_t*)&ui8ID, sizeof(TEEPROM::ui8ID)); 
    }
    return ui8ID;
}

void CEeprom::setHWVersion(const char* pcVersion) {
    if (!boDeviceInfoLoaded) {
        // If device info is not loaded, we should read it first
        readDeviceInfo(&tDeviceInfo);
    }
    strncpy(tDeviceInfo.acHWVersion, pcVersion, sizeof(tDeviceInfo.acHWVersion) - 1);
    tDeviceInfo.acHWVersion[sizeof(tDeviceInfo.acHWVersion) - 1] = '\0'; // Ensure null-termination
    writeDeviceInfo(&tDeviceInfo); // Write the updated device info back to EEPROM
}


void CEeprom::setSWVersion(const char* pcVersion) {
    if (!boDeviceInfoLoaded) {
        // If device info is not loaded, we should read it first
        readDeviceInfo(&tDeviceInfo);
    }
    strncpy(tDeviceInfo.acSWVersion, pcVersion, sizeof(tDeviceInfo.acSWVersion) - 1);
    tDeviceInfo.acSWVersion[sizeof(tDeviceInfo.acSWVersion) - 1] = '\0'; // Ensure null-termination
    writeDeviceInfo(&tDeviceInfo); // Write the updated device info back to EEPROM
}

bool CEeprom::writeUserData(const void* pvBuffer, uint16_t ui16Length) {
    // Write user data starting from address sizeof(TEEPROM) to avoid overwriting device info
    if (ui16Length + sizeof(TEEPROM) > I2C_DEVICESIZE_24LC02) {
        // Handle error: not enough space to write user data
        return false;
    }
    eeprom.writeBlock(sizeof(TEEPROM), (uint8_t*)pvBuffer, ui16Length);
    return true;
}

bool CEeprom::readUserData(const void* pvBuffer, uint16_t ui16Length) {
    // Read user data starting from address sizeof(TEEPROM) to avoid reading device info
    if (ui16Length + sizeof(TEEPROM) > I2C_DEVICESIZE_24LC02) {
        // Handle error: not enough space to read user data
        return false;
    }
    eeprom.readBlock(sizeof(TEEPROM), (uint8_t*)pvBuffer, ui16Length);
    return true;
}
} // namespace nspMiniCtrlBox