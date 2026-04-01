
#include <Adafruit_PWMServoDriver.h>

#define PCA_ADDRESS 0x40 // Base address for PCA9685 (plus the 3 address pins A0, A1, A2 via MCP23017)



class CServo {
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
    CServo(uint8_t baseAddress = PCA_ADDRESS);
    ~CServo();

    bool begin();
    bool isPresent();

    void setPWM(PWMChannel channel, uint16_t ui16ValueOn, uint16_t ui16ValueOff);
    void setPWM(PWMChannel channel, uint16_t ui16Value, bool boInvert = false);
    void setPWMPercent(PWMChannel channel, uint8_t ui8Percent, bool boInvert = false);
    void setPWMFreq(float fFreq);

    void allChannelsOff();
    
    void outputEnable(bool boEnable);

private:
    Adafruit_PWMServoDriver *pPwmDriver;
    uint8_t deviceAddress;
};