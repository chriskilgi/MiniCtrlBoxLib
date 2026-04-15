// asbSensors.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for asbSensors.cpp (AHT20, MLX90393, Analog)
// Neccessary for the AkorSensorBoard (ASB) of the MiniCtrlBox project.

#include <AHT20.h>
#include <7Semi_MLX90393.h>
#include "SparkFun_SCD4x_Arduino_Library.h"



#define PIN_ASB_SENSORS_ANALOG GPIO_NUM_0

namespace nspMiniCtrlBox {
    // The CASBSensors class provides methods to interact with the sensors (AHT20, MLX90393, Analog) connected to the AktorSensorBoard (ASB).
    class CASBSensors {
    public:
        // Struct to hold the magnetic sensor data
        struct TMLX90393Data {
                float x;
                float y;
                float z;
            };

        struct TSCD4xData {
            uint16_t ui16CO2;
            float fTemperature;
            float fHumidity;
        };

        CASBSensors();

        // Methods for the Humitity / Temperature AHT20 sensor
        bool initAHT20();
        float getAHT20Temperature();
        float getAHT20Humidity();

        // Methods for the magnetic sensor MLX90393
        bool initMLX90393();
        TMLX90393Data getMLX90393Data();

        // Methods for the SCD4x CO2 sensor
        bool initSCD4x();
        TSCD4xData getSCD4xData();

        // Methods for the analog Port GPIO0 (e.g. for a potentiometer)
        float getVoltageAnalog();
        float getVoltagePercent();
        uint16_t getVoltageDigital();

        void setReferenceVoltage(float fVoltage) { fRefVoltage = fVoltage; } // Setter for reference voltage

    private:
        AHT20 *pAHT20; // Pointer to the AHT20 sensor object, or nullptr if the sensor is not installed
        MLX90393_7Semi *pMLX90393; // Pointer to the MLX90393 sensor object, or nullptr if the sensor is not installed
        SCD4x *pSCD4x; // Pointer to the SCD4x sensor object, or nullptr if the sensor is not installed
        float fRefVoltage = 3.3f; // Default reference voltage for the potentiometer readings
    };
} // namespace nspMiniCtrlBox
