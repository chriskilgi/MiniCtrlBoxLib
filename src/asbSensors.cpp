#include <asbSensors.h>

CASBSensors::CASBSensors() {
    // Initialize the AHT20 sensor pointer to nullptr
    // (indicating that the sensor is not initiaLlized or not present)
    pAHT20 = nullptr; 
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

    pMLX90393 = new Adafruit_MLX90393();
    if (pMLX90393->begin_I2C()) {
        boSensorFound = true;
    } else {
        delete pMLX90393;
        pMLX90393 = nullptr;
    }

    return boSensorFound;
}

CASBSensors::TMLX90393Data CASBSensors::getMLX90393Data() {
    TMLX90393Data data = {0.0f, 0.0f, 0.0f}; // Default values if the sensor is not installed

    if (pMLX90393) {
        pMLX90393->readData(&data.x, &data.y, &data.z); // Read the magnetic field data from the sensor
    }

    return data;
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