#include <glo.h>
#include <pca.h>

namespace nspMiniCtrlBox {
CServo::CServo() {
    pPwmDriver = nullptr; // Initialize the PWM driver pointer to nullptr
}

CServo::~CServo() {
    if (pPwmDriver != nullptr) {
        delete pPwmDriver; // Clean up the PWM driver instance
        pPwmDriver = nullptr; // Set the pointer to nullptr after deletion
    }
}

bool CServo::isPresent() {
    return pPwmDriver != nullptr; // The MCP is considered present if the pointer is not null
}

bool CServo::begin() {
    Wire.begin(); // Initialize I2C communication
    
    if (!gloIsI2CDevicePresent(PCA_ADDRESS)) { // Check if the PCA9685 device is present at the specified I2C address
        return false; // If the device is not present, exit the function (pPwmDriver will remain nullptr to indicate that the device is not available)
    } else {
        pPwmDriver = new Adafruit_PWMServoDriver(PCA_ADDRESS); // Create an instance of the Adafruit_PWMServoDriver with the specified base address
    }

    pinMode(PIN_OUTPUT_ENABLE, OUTPUT); // Set the OE pin as an output
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

// Method to set the pulse width in microseconds for a specific channel
// This is particularly useful for controlling servos, where the pulse width directly corresponds to the servo position
// The PCA9685 can generate pulse widths from approximately 500 microseconds (0.5 ms) to 2500 microseconds (2.5 ms),
// which typically corresponds to the full range of motion for standard servos (0° to 180°)
void CServo::setMicroseconds(PWMChannel channel, uint16_t ui16Microseconds) {
    if (pPwmDriver == nullptr) {
        return;
    }
    pPwmDriver->writeMicroseconds(channel, ui16Microseconds);
}

// Method to convert an angle in degrees (0-180°) to a pulse width in microseconds based on the minimum and maximum pulse widths defined for the servo
// This method allows for easy control of servo position by specifying the desired angle, and it will calculate the corresponding pulse width to send to the servo
// The default minimum and maximum pulse widths are set to 500 and 2500 microseconds, respectively, which are common values for standard servos,
// but they can be adjusted using the setMinMaxServoMicroseconds method if your specific servo
uint16_t CServo::angleToMicroseconds(float fAngle) {
  // Begrenzen auf 0–180°
  if (fAngle < 0) fAngle = 0;
  if (fAngle > 180) fAngle = 180;

  // Calculate the pulse width in microseconds based on the angle, using a linear mapping between the minimum and maximum pulse widths defined for the servo
  // Normally, the values 500 and 2500 are used for standard servos, but they can be adjusted if your specific servo has different timing requirements
  // The values can be adjusted using the setMinMaxServoMicroseconds method to ensure compatibility with different servo models
  return ui16ServoTimeMin + (fAngle / 180.0f) * (ui16ServoTimeMax - ui16ServoTimeMin);
}

// Method to enable or disable the output of the PCA9685 by controlling the OE pin
// The OE pin is active low, meaning that setting it LOW enables the outputs, while setting it HIGH disables them
// This method allows for quickly turning off all outputs without having to set each channel to 0
// The default state is enabled (outputs active) when the begin() method is called, but this method can be used to disable the outputs when needed (e.g., for power saving or safety reasons)   
void CServo::outputEnable(bool boEnable) {
    if (pPwmDriver == nullptr) {
        return;
    }
    digitalWrite(PIN_OUTPUT_ENABLE, boEnable ? LOW : HIGH); // Set OE pin low to enable, high to disable
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
} // namespace nspMiniCtrlBox
