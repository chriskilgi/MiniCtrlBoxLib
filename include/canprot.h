// CanProtocol.h
#pragma once
#include <stdint.h>

// 11-Bit-ID: [ 2 Bit Prio | 5 Bit Node | 4 Bit MessageType ]
// MSB ............... LSB
// 10 00000 0000

namespace canproto {

enum class Priority : uint8_t {
    High    = 0b00, // Safety, Not-Aus, Heartbeat
    Medium  = 0b01, // Sensorwerte
    Low     = 0b10, // Aktorbefehle
    Debug   = 0b11  // Logging, Experimente
};

enum class MessageType : uint8_t {
    HeartbeatSystem      = 0b0000, // 0x0
    GpioOutDigitalLed    = 0b0001, // 0x1
    GpioOutDigitalRelay  = 0b0010, // 0x2
    GpioOutPwmRgb        = 0b0011, // 0x3
    GpioOutPwmServo      = 0b0100, // 0x4
    I2cOut               = 0b0101, // 0x5
    SpiOut               = 0b0110, // 0x6
    Reserved_0111        = 0b0111, // 0x7
    Reserved_1000        = 0b1000, // 0x8
    GpioInDigital        = 0b1001, // 0x9
    GpioInAnalog         = 0b1010, // 0xA
    Reserved_1011        = 0b1011, // 0xB
    Reserved_1100        = 0b1100, // 0xC
    I2cIn                = 0b1101, // 0xD
    SpiIn                = 0b1110, // 0xE
    Reserved_1111        = 0b1111  // 0xF
};

struct CanIdFields {
    Priority    prio;
    uint8_t     node;       // 0–31
    MessageType type;
};

inline uint16_t encodeId(const CanIdFields& f) {
    uint16_t id = 0;
    id |= (static_cast<uint16_t>(f.prio) & 0x03) << 9;      // Bits 10..9
    id |= (static_cast<uint16_t>(f.node) & 0x1F) << 4;      // Bits 8..4
    id |= (static_cast<uint16_t>(f.type) & 0x0F);           // Bits 3..0
    return id;
}

inline CanIdFields decodeId(uint16_t id) {
    CanIdFields f;
    f.prio = static_cast<Priority>((id >> 9) & 0x03);
    f.node = static_cast<uint8_t>((id >> 4) & 0x1F);
    f.type = static_cast<MessageType>(id & 0x0F);
    return f;
}

struct CanMessage {
    uint16_t    id;         // 11-Bit-ID
    uint8_t     dlc;        // 0–8
    uint8_t     data[8];
};

} // namespace canproto
