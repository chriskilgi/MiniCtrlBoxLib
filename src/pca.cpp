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

    pinMode(GPIO_NUM_7, OUTPUT); // Set GPIO 7 as OE pin for the PCA9685 PWM driver
    digitalWrite(GPIO_NUM_7, LOW); // Set OE pin low to enable the PCA9685 PWM driver

    pPwmDriver->begin(); // Initialize the PCA9685 PWM driver
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
void CServo::setPWM(PWMChannel channel, uint16_t on, uint16_t off) {
    if (pPwmDriver == nullptr) {
        return;
    }
    pPwmDriver->setPWM(channel, on, off);
}