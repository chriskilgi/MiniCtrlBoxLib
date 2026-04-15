#include <asbSensors.h>

namespace nspMiniCtrlBox {

// Alias for the magnetic sensor data struct
using TMLX90393Data = CASBSensors::TMLX90393Data;

// -------------------------------------------------------------
// Constructor
// -------------------------------------------------------------
CASBSensors::CASBSensors() {
    pAHT20 = nullptr;
    pMLX90393 = nullptr;
    pSCD41 = nullptr;
}

// -------------------------------------------------------------
// AHT20
// -------------------------------------------------------------
float CASBSensors::getAHT20Temperature() {
    if (!pAHT20) {
        return -273.15f;
    }
    return pAHT20->getTemperature();
}

float CASBSensors::getAHT20Humidity() {
    if (!pAHT20) {
        return -100.0f;
    }
    return pAHT20->getHumidity();
}

bool CASBSensors::initAHT20() {
    bool boSensorFound = false;

    pAHT20 = new AHT20();
    if (pAHT20->begin()) {
        boSensorFound = true;
    } else {
        delete pAHT20;
        pAHT20 = nullptr;
    }

    return boSensorFound;
}

// -------------------------------------------------------------
// MLX90393
// -------------------------------------------------------------
bool CASBSensors::initMLX90393() {
    bool boSensorFound = false;

    pMLX90393 = new MLX90393_7Semi();
    if (pMLX90393->begin(0x0C, Wire, 100000)) {
        boSensorFound = true;
    } else {
        delete pMLX90393;
        pMLX90393 = nullptr;
    }

    return boSensorFound;
}

TMLX90393Data CASBSensors::getMLX90393Data() {
    TMLX90393Data tMagneticData = {0.0f, 0.0f, 0.0f};

    if (!pMLX90393) {
        return tMagneticData;
    }

    pMLX90393->readMag(tMagneticData.x, tMagneticData.y, tMagneticData.z);
    return tMagneticData;
}

// -------------------------------------------------------------
// Analog Input
// -------------------------------------------------------------
uint16_t CASBSensors::getVoltageDigital() {
    return analogRead(PIN_ASB_SENSORS_ANALOG);
}

float CASBSensors::getVoltageAnalog() {
    uint16_t rawValue = getVoltageDigital();
    return (rawValue * fRefVoltage) / 4095.0f;
}

float CASBSensors::getVoltagePercent() {
    uint16_t rawValue = getVoltageDigital();
    return (rawValue * 100.0f) / 4095.0f;
}

// -------------------------------------------------------------
// SCD41 (CO₂ / Temp / Humidity)
// -------------------------------------------------------------
bool CASBSensors::initSCD41() {
    bool boSensorFound = false;

    pSCD41 = new SensirionI2cScd4x();
    pSCD41->begin(Wire, 0x62);

    uint16_t error;
    char errorMessage[256];

    // Stop any previous measurement
    error = pSCD41->stopPeriodicMeasurement();
    delay(500);

    // Start periodic measurement
    error = pSCD41->startPeriodicMeasurement();
    if (error) {
        // Fehlertext in char-array kopieren
        //strncpy(sSCD41LastError, errorMessage, sizeof(sSCD41LastError) - 1);
        //sSCD41LastError[sizeof(sSCD41LastError) - 1] = '\0';

        delete pSCD41;
        pSCD41 = nullptr;
        bSCD41Initialized = false;
        return false;
    }

    bSCD41Initialized = true;
    return true;
}

CASBSensors::TSCD41Data CASBSensors::getSCD41Data() {
    TSCD41Data data{};
    data.valid = false;

    if (!pSCD41 || !bSCD41Initialized) {
        strncpy(sSCD41LastError, "SCD41 not initialized", sizeof(sSCD41LastError) - 1);
        sSCD41LastError[sizeof(sSCD41LastError) - 1] = '\0';
        return data;
    }

    uint16_t co2;
    float temp;
    float hum;
    uint16_t error;
    char errorMessage[256];

    error = pSCD41->readMeasurement(co2, temp, hum);
    if (error) {
        //SensirionI2cScd4x::errorToString(error, errorMessage, sizeof(errorMessage));

        //strncpy(sSCD41LastError, errorMessage, sizeof(sSCD41LastError) - 1);
        //sSCD41LastError[sizeof(sSCD41LastError) - 1] = '\0';

        return data;
    }

    if (co2 == 0) {
        strncpy(sSCD41LastError, "Invalid sample", sizeof(sSCD41LastError) - 1);
        sSCD41LastError[sizeof(sSCD41LastError) - 1] = '\0';
        return data;
    }

    data.co2ppm = co2;
    data.temperature = temp;
    data.humidity = hum;
    data.valid = true;

    // Fehlertext löschen
    sSCD41LastError[0] = '\0';

    return data;
}

const char* CASBSensors::getSCD41LastError() const {
    return sSCD41LastError;
}

} // namespace nspMiniCtrlBox