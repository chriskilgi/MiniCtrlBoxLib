// asbSensors.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for asbSensors.cpp (AHT20, MLX90393, Analog)
// Neccessary for the AkorSensorBoard (ASB) of the MiniCtrlBox project.

#include <AHT20.h>              // for temperature and humidity sensor
#include <7Semi_MLX90393.h>     // for magnetic sensor  // for CO2-Sensor
#include <SensirionI2cScd4x.h>



#define PIN_ASB_SENSORS_ANALOG GPIO_NUM_0

namespace nspMiniCtrlBox {

    // The CASBSensors class provides methods to interact with the sensors
    // (AHT20, MLX90393, Analog, SCD41) connected to the AktorSensorBoard (ASB).
    class CASBSensors {
    public:

        // Struct to hold the magnetic sensor data
        struct TMLX90393Data {
            float x;
            float y;
            float z;
        };

        // Struct to hold SCD41 measurement data
        struct TSCD41Data {
            uint16_t co2ppm;
            float temperature;
            float humidity;
            bool valid;
        };

        CASBSensors();

        // --- AHT20 (Humidity / Temperature) ---
        bool initAHT20();
        float getAHT20Temperature();
        float getAHT20Humidity();

        // --- MLX90393 (Magnetic Sensor) ---
        bool initMLX90393();
        TMLX90393Data getMLX90393Data();

        // --- Analog Port GPIO0 (Potentiometer etc.) ---
        float getVoltageAnalog();
        float getVoltagePercent();
        uint16_t getVoltageDigital();
        void setReferenceVoltage(float fVoltage) { fRefVoltage = fVoltage; }

        // --- SCD41 (CO₂ / Temperature / Humidity) ---
        bool initSCD41();
        TSCD41Data getSCD41Data();
        const char* getSCD41LastError() const;

    private:
        // Existing sensors
        AHT20* pAHT20 = nullptr;
        MLX90393_7Semi* pMLX90393 = nullptr;

        // New: SCD41 sensor
        SensirionI2cScd4x* pSCD41 = nullptr;
        
        bool bSCD41Initialized = false;
        char sSCD41LastError[64] = {0};

        float fRefVoltage = 3.3f; // Default reference voltage
    };

} // namespace nspMiniCtrlBox

