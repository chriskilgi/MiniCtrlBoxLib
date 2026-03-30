#pragma once
#include <MCP23017.h>

#define MCP_LOCAL_ADDRESS 0x20  // Address for the MCP23017 on the Mainboard
#define MCP_SLB_ADDRESS 0x21    // Address for the MCP23017 on the SwitchLEDBoard

#define MINICTRLBOX_VERSION_GLOB MINICTRLBOX_VERSION

class CMCP {
    public:
        CMCP(uint8_t mcpAddress);
        ~CMCP();

        // Pure virtual function to be implemented by derived classes for specific initialization
        virtual bool begin() = 0; 

        bool isPresent(); // Check if the MCP23017 is present on the I2C bus

    protected:
        uint8_t deviceAddress;
        MCP23017 *pMCP;
};


// The CMCPLOCAL class inherits from CMCP and provides specific functionality for controlling
// the RGB LEDs connected to the MCP23017 on the Mainboard
class CPortExpLoc : public CMCP {
    public:
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
        CPortExpLoc(uint8_t mcpAddress);
        ~CPortExpLoc();

        bool begin() override;
        void setColor(RGBLEDColor tLEDColor, bool boState);

    private:
};

// The CMCPSLB class inherits from CMCP and provides specific functionality for controlling
// the LEDs and the switches connected to the MCP23017 on the SwitchLEDBoard
class CPortExpRem : public CMCP {
    public:
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
        CPortExpRem(uint8_t mcpAddress);
        ~CPortExpRem();

        bool begin() override;
        uint8_t getSwitchState(); // Function to read the state of the switches on the SwitchLEDBoard
        void setLED(LEDColor tLEDColor, bool boState);
        void setLEDPort(uint8_t uiState);
};

// Bitwise OR operator overload for RGBLEDColor combining LED colors
inline CPortExpLoc::RGBLEDColor operator|(CPortExpLoc::RGBLEDColor a, CPortExpLoc::RGBLEDColor b) {
    return static_cast<CPortExpLoc::RGBLEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

// Bitwise OR operator overload for LEDColor to allow combining multiple LEDs
inline CPortExpRem::LEDColor operator|(CPortExpRem::LEDColor a, CPortExpRem::LEDColor b) {
    return static_cast<CPortExpRem::LEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}