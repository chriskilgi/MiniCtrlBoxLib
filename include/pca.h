
#include <Adafruit_PWMServoDriver.h>

#define PCA_ADDRESS 0x40 // Base address for PCA9685 (plus the 3 address pins A0, A1, A2 via MCP23017)
#define PIN_OUTPUT_ENABLE GPIO_NUM_7 // GPIO pin used to control the OE pin of the PCA9685


class CServo {
public:
    // The PWMChannel enum defines the identifiers for the 8 PWM channels of the PCA9685.
    // The first 6 channels (0-5) are used for controlling the RGB LEDs,
    // while the last 2 channels (6-7) are used for controlling the two servos
    enum PWMChannel : uint8_t {
        RED0 = 0,
        GREEN0 = 1,
        BLUE0 = 2,
        RED1 = 3,
        GREEN1 = 4,
        BLUE1 = 5,
        SERVO1 = 6,
        SERVO2 = 7
    };
    // The SensorPin enum defines the identifiers for the two sensors connected to the servo board.
    // These identifiers can be used in the getServoSensorDigital and getServoSensorAnalog methods
    // to specify which sensor's state or voltage to read. 
    enum SensorPin : uint8_t {
        SENSOR1 = 1,
        SENSOR2 = 2
    };

    CServo();
    ~CServo();

    bool begin();
    bool isPresent();

    // Functions for setting PWM values
    void setPWM(PWMChannel channel, uint16_t ui16ValueOn, uint16_t ui16ValueOff);
    void setPWM(PWMChannel channel, uint16_t ui16Value, bool boInvert = false);
    void setPWMPercent(PWMChannel channel, uint8_t ui8Percent, bool boInvert = false);
    void setPWMFreq(float fFreq);
    void setMicroseconds(PWMChannel channel, uint16_t ui16Microseconds);
    uint16_t angleToMicroseconds(float angle);
    void setMinMaxServoMicroseconds(uint16_t ui16Min, uint16_t ui16Max) { ui16ServoTimeMin = ui16Min; ui16ServoTimeMax = ui16Max; } // Setter for minimum and maximum pulse widths for the servo

    void allChannelsOff();
    void outputEnable(bool boEnable);

    // Functions for reading potentiometer voltage
    uint16_t getPotiVoltageDigital();
    float getPotiVoltageAnalog();
    float getPotiVoltagePercent();
    // For a more precise voltage reading, you can use the following function
    // to convert the ADC value to voltage based on the reference voltage (to be measured and set by the user)
    void setReferenceVoltage(float fVoltage) { fRefVoltage = fVoltage; } // Setter for reference voltage

    // Functions for reading servo sensor states
    bool getServoSensorDigital(SensorPin ui8SensorPin);
    float getServoSensorAnalog(SensorPin ui8SensorPin);

private:
    Adafruit_PWMServoDriver *pPwmDriver; // Pointer to the PCA9685 driver instance, initialized in begin() if the device is present
    float fRefVoltage = 3.3; // Reference voltage for ADC conversion (adjust if using a different reference voltage)
    uint16_t ui16ServoTimeMin = 500; // Minimum pulse width in microseconds (0.5 ms)
    uint16_t ui16ServoTimeMax = 2500; // Maximum pulse width in microseconds (2.5 ms)
};