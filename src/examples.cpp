#include "canprot.h"

using namespace canproto;

CanMessage makeLedSet(uint8_t node, uint8_t ledIndex, bool on) {
    CanIdFields f{
        Priority::Low,
        node,
        MessageType::GpioOutDigitalLed
    };

    CanMessage msg{};
    msg.id  = encodeId(f);
    msg.dlc = 2;
    msg.data[0] = ledIndex;
    msg.data[1] = on ? 1 : 0;
    return msg;
}

CanMessage makeServoSet(uint8_t node, uint8_t servoIndex, uint8_t angleDeg) {
    CanIdFields f{
        Priority::Low,
        node,
        MessageType::GpioOutPwmServo
    };

    CanMessage msg{};
    msg.id  = encodeId(f);
    msg.dlc = 3;
    msg.data[0] = 0;          // Kommando: 0 = Setze Winkel
    msg.data[1] = servoIndex; // Servo-Nummer
    msg.data[2] = angleDeg;   // 0–180
    return msg;
}

CanMessage makeI2cRead(uint8_t node, uint8_t addr, uint8_t reg, uint8_t len) {
    CanIdFields f{
        Priority::Medium,
        node,
        MessageType::I2cOut
    };

    CanMessage msg{};
    msg.id  = encodeId(f);
    msg.dlc = 4;
    msg.data[0] = 0;      // Kommando: 0 = Read
    msg.data[1] = addr;   // 7-Bit-Adresse
    msg.data[2] = reg;    // Register
    msg.data[3] = len;    // Anzahl Bytes
    return msg;
}

// MessageType::I2cIn
// data[0] = Status (0 = OK, !=0 Fehler)
// data[1] = addr
// data[2] = reg
// data[3..] = Nutzdaten




