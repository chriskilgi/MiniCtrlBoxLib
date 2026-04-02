#include <glo.h>
#include <pca.h>

CServo::CServo(uint8_t baseAddress) {
    pPwmDriver = nullptr; // Initialize the PWM driver pointer to nullptr
    this->deviceAddress = baseAddress; // Store the base address for later use in case we need to reinitialize the driver
}

CServo::~CServo() {
    delete pPwmDriver;
    pPwmDriver = nullptr;
}

bool CServo::isPresent() {
    return pPwmDriver != nullptr; // The MCP is considered present if the pointer is not null
}

bool CServo::begin() {
    Wire.begin(); // Initialize I2C communication
    
    if (!gloIsI2CDevicePresent(deviceAddress)) { // Check if the MCP23017 device is present at the specified I2C address
        return false; // If the device is not present, exit the function (pMCP will remain nullptr to indicate that the device is not available)
    } else {
        pPwmDriver = new Adafruit_PWMServoDriver(deviceAddress); // Create an instance of the Adafruit_PWMServoDriver with the specified base address
    }

    pinMode(GPIO_NUM_7, OUTPUT); // Set GPIO 7 as an output for controlling the OE pin of the PCA9685
    outputEnable(true); // Enable the output of the PCA9685 by setting the OE pin low

    pPwmDriver->begin(); // Initialize the PCA9685 PWM driver
    delay(200); // Wait for the PCA9685 to initialize properly

    pPwmDriver->setPWMFreq(1000); // Set frequency to 1 kHz for LED dimming

    return true;
}

void CServo::allChannelsOff() {
    if (pPwmDriver == nullptr) {
        return;
    }
    for (uint8_t i = 0; i < 8; i++) {
        pPwmDriver->setPWM(i, 0, 0);
    }
}
void CServo::setPWM(PWMChannel channel, uint16_t ui16ValueOn, uint16_t ui16ValueOff) {
    if (pPwmDriver == nullptr) {
        return;
    }
    pPwmDriver->setPWM(channel, ui16ValueOn, ui16ValueOff);
}

void CServo::setPWM(PWMChannel channel, uint16_t ui16Value, bool boInvert) {
    if (pPwmDriver == nullptr) {
        return;
    }
    pPwmDriver->setPin(channel, ui16Value, boInvert);
}

void CServo::setPWMPercent(PWMChannel channel, uint8_t ui8Percent, bool boInvert) {
    if (pPwmDriver == nullptr) {
        return;
    }
    uint16_t ui16Value = (ui8Percent * 4095) / 100; // Convert percentage to a value between 0 and 4095
    pPwmDriver->setPin(channel, ui16Value, boInvert);
}

// Method to set the PWM frequency for the PCA9685
// The PCA9685 supports a frequency range of approximately 24 Hz to 1526 Hz, so the input frequency should be within this range for proper operation
// For LED dimming applications, a common frequency is around 1 kHz, which provides smooth dimming without visible flicker
// The default frequency is set to 1 kHz in the begin() method, but this method allows
// changing it as needed for different applications (e.g., servos may require a different frequency than LEDs)
void CServo::setPWMFreq(float fFreq) {
    if (pPwmDriver == nullptr) {
        return;
    }
    pPwmDriver->setPWMFreq(fFreq);
}

// Method to enable or disable the output of the PCA9685 by controlling the OE pin
// The OE pin is active low, meaning that setting it LOW enables the outputs, while setting it HIGH disables them
// This method allows for quickly turning off all outputs without having to set each channel to 0
// The default state is enabled (outputs active) when the begin() method is called, but this method can be used to disable the outputs when needed (e.g., for power saving or safety reasons)   
void CServo::outputEnable(bool boEnable) {
    if (pPwmDriver == nullptr) {
        return;
    }
    digitalWrite(GPIO_NUM_7, boEnable ? LOW : HIGH); // Set OE pin low to enable, high to disable
}

// Read the voltage from the potentiometer connected to GPIO 5 and return it
// as a 16-bit value (0-4095 for a 12-bit ADC)
uint16_t CServo::getPotiVoltageDigital() {
    return analogRead(GPIO_NUM_5); 
}

// Read the voltage from the potentiometer and return it as a float value in volts
// Assuming a reference voltage of 3.3V and a 12-bit ADC, the raw ADC value (0-4095) is converted to a voltage by multiplying it by the reference voltage and dividing by the maximum ADC value
float CServo::getPotiVoltageAnalog() {
    uint16_t rawValue = getPotiVoltageDigital(); // Get the raw ADC value
    return (rawValue * fRefVoltage) / 4095; // Convert the raw value to a voltage (0-3.3V)
}

// Read the voltage from the potentiometer and return it as a percentage (0-100%)
float CServo::getPotiVoltagePercent() {
    uint16_t rawValue = getPotiVoltageDigital(); // Get the raw ADC value (0-4095)
    return (rawValue * 100.0) / 4095.0; // Convert the raw value to a percentage
}

bool CServo::getServoSensorDigital(SensorPin ui8SensorPin) {
    switch (ui8SensorPin) {
        case SENSOR1:
            return digitalRead(GPIO_NUM_0); // Sensor 1 connected to GPIO 0
        case SENSOR2:
            return digitalRead(GPIO_NUM_1); // Sensor 2 connected to GPIO 1
        default:
            return false; // Return false for invalid sensor pin
    }
}

float CServo::getServoSensorAnalog(SensorPin ui8SensorPin) {
    uint16_t rawValue;
    switch (ui8SensorPin) {
        case SENSOR1:
            rawValue = analogRead(GPIO_NUM_0); // Sensor 1 connected to GPIO 0
            return (rawValue * fRefVoltage) / 4095; // Convert the raw value to a voltage (0-3.3V)
        case SENSOR2:
            rawValue = analogRead(GPIO_NUM_1); // Sensor 2 connected to GPIO 1
            return (rawValue * fRefVoltage) / 4095; // Convert the raw value to a voltage (0-3.3V)
        default:
            return 0.0; // Return 0 for invalid sensor pin
    }
}
