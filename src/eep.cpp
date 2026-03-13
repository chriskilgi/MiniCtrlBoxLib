#include <eep.h>

CEEPROM::CEEPROM() : eeprom(EEPROM_ADRESS, I2C_DEVICESIZE_24LC02)
{
    eeprom.begin();
}

bool CEEPROM::isConnected()
{
    return eeprom.isConnected();
}

uint8_t CEEPROM::getAddress()
{
    return eeprom.getAddress();
}

void CEEPROM::writeBlock(uint16_t memoryAddress, const uint8_t * buffer, uint16_t length)
{
    eeprom.writeBlock(memoryAddress, buffer, length);
}

void CEEPROM::readBlock(uint16_t memoryAddress, uint8_t * buffer, uint16_t length)
{
    eeprom.readBlock(memoryAddress, buffer, length);
}

void CEEPROM::writeDeviceInfo(const TEEPROM& deviceInfo) {
    tDeviceInfo = deviceInfo;
    writeBlock(0, (const uint8_t*)&deviceInfo, sizeof(TEEPROM));
}

void CEEPROM::readDeviceInfo(TEEPROM& deviceInfo) {
    readBlock(0, (uint8_t*)&deviceInfo, sizeof(TEEPROM));
    tDeviceInfo = deviceInfo; // Update local copy
    boDeviceInfoLoaded = true;
}

