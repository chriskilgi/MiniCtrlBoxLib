#include <eep.h>

namespace nspMiniCtrlBox {
CEeprom::CEeprom(uint8_t deviceAddress) : eeprom(deviceAddress, I2C_DEVICESIZE_24LC02)
{

}

bool CEeprom::isPresent(uint8_t ui8DeviceAddress) {
    return gloIsI2CDevicePresent(ui8DeviceAddress);
}

uint8_t CEeprom::getAddress()
{
    return eeprom.getAddress();
}

void CEeprom::writeDeviceInfo(const TEEPROM* pDeviceInfo) {
    if (isPresent()) {
        tDeviceInfo = *pDeviceInfo;
        eeprom.writeBlock(0, (const uint8_t*)pDeviceInfo, sizeof(TEEPROM));
    }
}

void CEeprom::readDeviceInfo(TEEPROM* pDeviceInfo) {
    if (isPresent()) {
        eeprom.readBlock(0, (uint8_t*)pDeviceInfo, sizeof(TEEPROM));
        tDeviceInfo = *pDeviceInfo; // Update local copy
        boDeviceInfoLoaded = true;
    }
}

void CEeprom::writeID(uint8_t ui8ID) {
    if (isPresent()) {
        eeprom.writeBlock(offsetof(TEEPROM, ui8ID), (const uint8_t*)&ui8ID, sizeof(TEEPROM::ui8ID)); // Write only the ID field to EEPROM
    }
}

uint8_t CEeprom::readID() {
    if (isPresent()) {
        // Read only the ID field from EEPROM
        eeprom.readBlock(offsetof(TEEPROM, ui8ID), (uint8_t*)&tDeviceInfo.ui8ID, sizeof(TEEPROM::ui8ID)); 
    }
    return tDeviceInfo.ui8ID;
}

void CEeprom::writeHWVersion(const char* pcVersion) {
    if (isPresent()) {
        // Write only the HWVersion field to EEPROM
        eeprom.writeBlock(offsetof(TEEPROM, acHWVersion), (const uint8_t*)pcVersion, sizeof(TEEPROM::acHWVersion)); 
    }
}

const char* CEeprom::readHWVersion() {
    if (isPresent()) {
        // Read only the HWVersion field from EEPROM
        eeprom.readBlock(offsetof(TEEPROM, acHWVersion), (uint8_t*)&tDeviceInfo.acHWVersion, sizeof(TEEPROM::acHWVersion)); 
    }
    return tDeviceInfo.acHWVersion;
}

void CEeprom::writeSWVersion(const char* pcVersion) {
    if (isPresent()) {
        // Write only the SWVersion field to EEPROM
        eeprom.writeBlock(offsetof(TEEPROM, acSWVersion), (const uint8_t*)pcVersion, sizeof(TEEPROM::acSWVersion)); 
    }
}

const char* CEeprom::readSWVersion() {
    if (isPresent()) {
        // Read only the SWVersion field from EEPROM
        eeprom.readBlock(offsetof(TEEPROM, acSWVersion), (uint8_t*)tDeviceInfo.acSWVersion, sizeof(TEEPROM::acSWVersion)); 
    }
    return tDeviceInfo.acSWVersion;
}

void CEeprom::writeProjectName(const char* pcName) {
    if (isPresent()) {
        // Write only the ProjectName field to EEPROM
        eeprom.writeBlock(offsetof(TEEPROM, acProjectname), (const uint8_t*)pcName, sizeof(TEEPROM::acProjectname)); 
    }
}

const char* CEeprom::readProjectName() {
    if (isPresent()) {
        // Read only the ProjectName field from EEPROM
        eeprom.readBlock(offsetof(TEEPROM, acProjectname), (uint8_t*)tDeviceInfo.acProjectname, sizeof(TEEPROM::acProjectname)); 
    }
    return tDeviceInfo.acProjectname;
}

void CEeprom::writeBoardName(const char* pcName) {
    if (isPresent()) {
        // Write only the BoardName field to EEPROM
        eeprom.writeBlock(offsetof(TEEPROM, acBoardname), (const uint8_t*)pcName, sizeof(TEEPROM::acBoardname)); 
    }
}

const char* CEeprom::readBoardName() {
    if (isPresent()) {
        // Read only the BoardName field from EEPROM
        eeprom.readBlock(offsetof(TEEPROM, acBoardname), (uint8_t*)tDeviceInfo.acBoardname, sizeof(TEEPROM::acBoardname)); 
    }
    return tDeviceInfo.acBoardname;
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