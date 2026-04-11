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

void CEeprom::setHWVersion(const char* version) {
    if (!boDeviceInfoLoaded) {
        // If device info is not loaded, we should read it first
        readDeviceInfo(&tDeviceInfo);
    }
    strncpy(tDeviceInfo.acHWVersion, version, sizeof(tDeviceInfo.acHWVersion) - 1);
    tDeviceInfo.acHWVersion[sizeof(tDeviceInfo.acHWVersion) - 1] = '\0'; // Ensure null-termination
    writeDeviceInfo(&tDeviceInfo); // Write the updated device info back to EEPROM
}


void CEeprom::setSWVersion(const char* version) {
    if (!boDeviceInfoLoaded) {
        // If device info is not loaded, we should read it first
        readDeviceInfo(&tDeviceInfo);
    }
    strncpy(tDeviceInfo.acSWVersion, version, sizeof(tDeviceInfo.acSWVersion) - 1);
    tDeviceInfo.acSWVersion[sizeof(tDeviceInfo.acSWVersion) - 1] = '\0'; // Ensure null-termination
    writeDeviceInfo(&tDeviceInfo); // Write the updated device info back to EEPROM
}

bool CEeprom::writeUserData(const uint8_t * buffer, uint16_t length) {
    // Write user data starting from address sizeof(TEEPROM) to avoid overwriting device info
    if (length + sizeof(TEEPROM) > I2C_DEVICESIZE_24LC02) {
        // Handle error: not enough space to write user data
        return false;
    }
    eeprom.writeBlock(sizeof(TEEPROM), buffer, length);
    return true;
}

bool CEeprom::readUserData(uint8_t * buffer, uint16_t length) {
    // Read user data starting from address sizeof(TEEPROM) to avoid reading device info
    if (length + sizeof(TEEPROM) > I2C_DEVICESIZE_24LC02) {
        // Handle error: not enough space to read user data
        return false;
    }
    eeprom.readBlock(sizeof(TEEPROM), buffer, length);
    return true;
}
} // namespace nspMiniCtrlBox