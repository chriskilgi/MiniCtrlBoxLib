#include <AHT20.h>
#include <Adafruit_MLX90393.h>


#define PIN_ASB_SENSORS_ANALOG GPIO_NUM_0

class CASBSensors {
public:
        struct TMLX90393Data {
            float x;
            float y;
            float z;
        };

    CASBSensors();

    // Methods for the AHT20 sensor
    bool initAHT20();
    float getAHT20Temperature();
    float getAHT20Humidity();

    // Methods for the magnetic sensor MLX90393
    bool initMLX90393();
    TMLX90393Data getMLX90393Data();

    // Methods for the analog Port GPIO0 (e.g. for a potentiometer)
    float getVoltageAnalog();
    float getVoltagePercent();
    uint16_t getVoltageDigital();

    void setReferenceVoltage(float fVoltage) { fRefVoltage = fVoltage; } // Setter for reference voltage

private:
    AHT20 *pAHT20; // Pointer to the AHT20 sensor object, or nullptr if the sensor is not installed
    Adafruit_MLX90393 *pMLX90393; // Pointer to the MLX90393 sensor object, or nullptr if the sensor is not installed
    float fRefVoltage = 3.3f; // Default reference voltage for the potentiometer readings
};
