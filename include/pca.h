
#include <Adafruit_PWMServoDriver.h>

#define PCA_ADDRESS 0x40 // Base address for PCA9685 (plus the 3 address pins A0, A1, A2 via MCP23017)



class CPWMDRIVER {
public:
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
    CPWMDRIVER();
    ~CPWMDRIVER();

    void begin();
    void setPWM(uint8_t channel, uint16_t on, uint16_t off);
    void allChannelsOff();

private:
    Adafruit_PWMServoDriver *pPwmDriver;
};