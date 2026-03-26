#pragma once
#include <MCP23017.h>

#define MCP_LOCAL_ADDRESS 0x20  // Address for the MCP23017 on the Mainboard
#define MCP_SLB_ADDRESS 0x21    // Address for the MCP23017 on the SwitchLEDBoard

#define MINICTRLBOX_VERSION_GLOB MINICTRLBOX_VERSION

// Konstanten für die RGB-LED-Farben auf dem Mainboard
enum RGBLEDColor : uint8_t {
    ALLCOLORS = 0b00111111,
    RED0 = 0b00000001,
    GREEN0 = 0b00000010,
    BLUE0 = 0b00000100,
    RED1 = 0b00001000,
    GREEN1 = 0b00010000,
    BLUE1 = 0b00100000
};

// Konstanten für die LEDs auf dem SwitchLEDBoard
enum LEDColor : uint8_t {
    ALLLEDS = 0b11111111,
    LED0 = 0b00000001,
    LED1 = 0b00000010,
    LED2 = 0b00000100,
    LED3 = 0b00001000,
    LED4 = 0b00010000,
    LED5 = 0b00100000,
    LED6 = 0b01000000,
    LED7 = 0b10000000
};

// Bitwise OR operator overload for RGBLEDColor combining LED colors
inline RGBLEDColor operator|(RGBLEDColor a, RGBLEDColor b) {
    return static_cast<RGBLEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

// Bitwise OR operator overload for LEDColor to allow combining multiple LEDs
inline LEDColor operator|(LEDColor a, LEDColor b) {
    return static_cast<LEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

class CMCP {
    public:
        CMCP(uint8_t mcpAddress);
        ~CMCP();

        // Pure virtual function to be implemented by derived classes for specific initialization
        virtual void begin() = 0; 

    protected:
        uint8_t mcpAddress;
        MCP23017 *pMCP;
};


// The CMCPLOCAL class inherits from CMCP and provides specific functionality for controlling
// the RGB LEDs connected to the MCP23017 on the Mainboard
class CMCPLOCAL : public CMCP {
    public:
        CMCPLOCAL(uint8_t mcpAddress);
        ~CMCPLOCAL();

        void begin() override;
        void setColor(RGBLEDColor tLEDColor, bool boState);

    private:
};

// The CMCPSLB class inherits from CMCP and provides specific functionality for controlling
// the LEDs and the switches connected to the MCP23017 on the SwitchLEDBoard
class CMCPSLB : public CMCP {
    public:
        CMCPSLB(uint8_t mcpAddress);
        ~CMCPSLB();

        void begin() override;
        void setLED(LEDColor tLEDColor, bool boState);

    private:
};

