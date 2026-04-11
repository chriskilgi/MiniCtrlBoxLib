#include <asbSensors.h>

namespace nspMiniCtrlBox {
// Alias for the magnetic sensor data struct, to avoid having to write CASBSensors::TMLX90393Data everywhere
using TMLX90393Data = CASBSensors::TMLX90393Data;

CASBSensors::CASBSensors() {
    // Initialize the AHT20 sensor pointer to nullptr
    // (indicating that the sensor is not initiaLlized or not present)
    pAHT20 = nullptr; 

    // Initialize the MLX90393 sensor pointer to nullptr
    // (indicating that the sensor is not initiaLlized or not present)
    pMLX90393 = nullptr;
}


// Return the temperature value from the AHT20 sensor, or -273.15 if the sensor is not installed
float CASBSensors::getAHT20Temperature() {
    if (!pAHT20) {
        return -273.15f; // Return a very low temperature if the sensor is not installed :-)
    }
    return pAHT20->getTemperature();
}

// Return the humidity value from the AHT20 sensor, or -1 if the sensor is not installed
float CASBSensors::getAHT20Humidity() {
    if (!pAHT20) {
        return -100.0f; // Return an impossible value if the sensor is not installed
    }
    return pAHT20->getHumidity();
}

// Try to initialize the AHT20 sensor and return true if successful, false otherwise
// This is important, because if the sensor ist not plugged in, we don't want to try
// to read data from it, which would cause errors
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
    TMLX90393Data tMagneticData = {0.0f, 0.0f, 0.0f}; // Default values if the sensor is not installed

    if (!pMLX90393) {
        return tMagneticData; // Return default values if the sensor is not installed
    } else {
        // Read the magnetic field data from the MLX90393 sensor and store it in the data struct
        pMLX90393->readMag(tMagneticData.x, tMagneticData.y, tMagneticData.z);
    }
    return tMagneticData;
}

// Read the voltage from the potentiometer connected to GPIO 5 and return it
// as a 16-bit value (0-4095 for a 12-bit ADC)
uint16_t CASBSensors::getVoltageDigital() {
    return analogRead(PIN_ASB_SENSORS_ANALOG); 
}

// Read the voltage from the potentiometer and return it as a float value in volts
// Assuming a reference voltage of 3.3V and a 12-bit ADC, the raw ADC value (0-4095) is converted to a voltage by multiplying it by the reference voltage and dividing by the maximum ADC value
float CASBSensors::getVoltageAnalog() {
    uint16_t rawValue = getVoltageDigital(); // Get the raw ADC value
    return (rawValue * fRefVoltage) / 4095; // Convert the raw value to a voltage (0-3.3V)
}

// Read the voltage from the potentiometer and return it as a percentage (0-100%)
float CASBSensors::getVoltagePercent() {
    uint16_t rawValue = getVoltageDigital(); // Get the raw ADC value (0-4095)
    return (rawValue * 100.0) / 4095.0; // Convert the raw value to a percentage
}

} // namespace nspMiniCtrlBox