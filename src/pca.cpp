#include <pca.h>

CPWMDRIVER::CPWMDRIVER() {

    pPwmDriver = new Adafruit_PWMServoDriver(PCA_ADDRESS); // Create an instance of the Adafruit_PWMServoDriver with the base address 0x40 (the actual address will be set via the MCP23017)
}

CPWMDRIVER::~CPWMDRIVER() {
    delete pPwmDriver;
    pPwmDriver = nullptr;
}

void CPWMDRIVER::begin() {
    pinMode(GPIO_NUM_7, OUTPUT); // Set GPIO 7 as OE pin for the PCA9685 PWM driver
    digitalWrite(GPIO_NUM_7, LOW); // Set OE pin low to enable the PCA9685 PWM driver

    pPwmDriver->begin(); // Initialize the PCA9685 PWM driver
    pPwmDriver->setPWMFreq(1000); // Set frequency to 1 kHz for LED dimming
}

void CPWMDRIVER::allChannelsOff() {
    for (uint8_t i = 0; i < 8; i++) {
        pPwmDriver->setPWM(i, 0, 0);
    }
}
void CPWMDRIVER::setPWM(uint8_t channel, uint16_t on, uint16_t off) {
    pPwmDriver->setPWM(channel, on, off);
}