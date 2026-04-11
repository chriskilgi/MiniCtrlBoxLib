// CanParser.h
#pragma once
#include "canprot.h"

namespace canproto {

class CanParser {
public:
    void handleFrame(const CanMessage& msg) {
        auto f = decodeId(msg.id);

        switch (f.type) {
        case MessageType::HeartbeatSystem:
            handleHeartbeat(f, msg);
            break;
        case MessageType::GpioOutDigitalLed:
            handleGpioOutLed(f, msg);
            break;
        case MessageType::GpioOutDigitalRelay:
            handleGpioOutRelay(f, msg);
            break;
        case MessageType::GpioOutPwmRgb:
            handlePwmRgb(f, msg);
            break;
        case MessageType::GpioOutPwmServo:
            handlePwmServo(f, msg);
            break;
        case MessageType::I2cOut:
            handleI2cOut(f, msg);
            break;
        case MessageType::SpiOut:
            handleSpiOut(f, msg);
            break;
        case MessageType::GpioInDigital:
            handleGpioInDigital(f, msg);
            break;
        case MessageType::GpioInAnalog:
            handleGpioInAnalog(f, msg);
            break;
        case MessageType::I2cIn:
            handleI2cIn(f, msg);
            break;
        case MessageType::SpiIn:
            handleSpiIn(f, msg);
            break;
        default:
            handleReserved(f, msg);
            break;
        }
    }

protected:
    // Diese Methoden überschreibst du in abgeleiteten Klassen
    virtual void handleHeartbeat(const CanIdFields&, const CanMessage&) {}
    virtual void handleGpioOutLed(const CanIdFields&, const CanMessage&) {}
    virtual void handleGpioOutRelay(const CanIdFields&, const CanMessage&) {}
    virtual void handlePwmRgb(const CanIdFields&, const CanMessage&) {}
    virtual void handlePwmServo(const CanIdFields&, const CanMessage&) {}
    virtual void handleI2cOut(const CanIdFields&, const CanMessage&) {}
    virtual void handleSpiOut(const CanIdFields&, const CanMessage&) {}
    virtual void handleGpioInDigital(const CanIdFields&, const CanMessage&) {}
    virtual void handleGpioInAnalog(const CanIdFields&, const CanMessage&) {}
    virtual void handleI2cIn(const CanIdFields&, const CanMessage&) {}
    virtual void handleSpiIn(const CanIdFields&, const CanMessage&) {}
    virtual void handleReserved(const CanIdFields&, const CanMessage&) {}
};

} // namespace canproto
