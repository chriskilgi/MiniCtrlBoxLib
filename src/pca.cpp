#include <pca.h>

CPCA9685::CPCA9685() {
    pPwmDriver = new Adafruit_PWMServoDriver(PCA_ADDRESS); // Create an instance of the Adafruit_PWMServoDriver with the base address 0x40 (the actual address will be set via the MCP23017)
}

CPCA9685::~CPCA9685() {
    delete pPwmDriver;
    pPwmDriver = nullptr;
}

void CPCA9685::begin() {
    if (pPwmDriver == nullptr) {
        return;
    }
    pinMode(GPIO_NUM_7, OUTPUT); // Set GPIO 7 as OE pin for the PCA9685 PWM driver
    digitalWrite(GPIO_NUM_7, LOW); // Set OE pin low to enable the PCA9685 PWM driver

    pPwmDriver->begin(); // Initialize the PCA9685 PWM driver
    pPwmDriver->setPWMFreq(1000); // Set frequency to 1 kHz for LED dimming
}

void CPCA9685::allChannelsOff() {
    if (pPwmDriver == nullptr) {
        return;
    }
    for (uint8_t i = 0; i < 8; i++) {
        pPwmDriver->setPWM(i, 0, 0);
    }
}
void CPCA9685::setPWM(uint8_t channel, uint16_t on, uint16_t off) {
    if (pPwmDriver == nullptr) {
        return;
    }
    pPwmDriver->setPWM(channel, on, off);
}