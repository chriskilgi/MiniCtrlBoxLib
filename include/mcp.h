#pragma once
#include <MCP23017.h>

#define MCP_LOCAL_ADDRESS 0x20  // Address for the MCP23017 on the Mainboard
#define MCP_SLB_ADDRESS 0x21    // Address for the MCP23017 on the SwitchLEDBoard

#define MINICTRLBOX_VERSION_GLOB MINICTRLBOX_VERSION

class CMCP {
    public:
        CMCP(uint8_t ui8MCPAddress);
        ~CMCP();

        // Pure virtual function to be implemented by derived classes for specific initialization
        virtual bool begin() = 0; 

        bool isPresent(); // Check if the MCP23017 is present on the I2C bus

    protected:
        uint8_t ui8MCPAddress; // I2C address of the MCP23017
        MCP23017 *pMCP; // Pointer to the MCP23017 instance, initialized in derived classes if the device is present
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
        CPortExpLoc(uint8_t ui8MCPAddress = MCP_LOCAL_ADDRESS);
        ~CPortExpLoc();

        bool begin() override;

        // Function to set the state of the RGB LEDs based on the specified color and state
        void setColor(RGBLEDColor tLEDColor, bool boState);
        // Overloaded function to set the state of the RGB LEDs based on a color index (0-5) and state
        void setColor(uint8_t ui8Color, bool boState);
};

// The CMCPSLB class inherits from CMCP and provides specific functionality for controlling
// the LEDs and the switches connected to the MCP23017 on the SwitchLEDBoard
class CPortExpRem : public CMCP {
    public:
        // Konstanten für die LEDs auf dem SwitchLEDBoard
        enum LEDColor : uint8_t {
            ALLLEDS = 0b11111111,
            LEDRE0 = 0b00000001,
            LEDYE0 = 0b00000010,
            LEDGN0 = 0b00000100,
            LEDBL0 = 0b00001000,
            LEDRE1 = 0b00010000,
            LEDYE1 = 0b00100000,
            LEDGN1 = 0b01000000,
            LEDBL1 = 0b10000000
        };
        CPortExpRem(uint8_t ui8MCPAddress = MCP_SLB_ADDRESS);
        ~CPortExpRem();

        bool begin() override;
        
        void setLED(LEDColor tLEDColor, bool boState);
        void setLED(uint8_t ui8LED, bool boState);
        void setLEDPort(uint8_t ui8PortState); // Function to set the state of all LEDs on the SwitchLEDBoard based on a port state byte

        uint8_t getSwitchState(); // Function to read the state of the switches on the SwitchLEDBoard
        bool getSwitchState(uint8_t ui8SwitchNo); // Overloaded function to check if specific switches are pressed based on a switch index (0-7)
};

// Bitwise OR operator overload for RGBLEDColor combining LED colors
inline CPortExpLoc::RGBLEDColor operator|(CPortExpLoc::RGBLEDColor a, CPortExpLoc::RGBLEDColor b) {
    return static_cast<CPortExpLoc::RGBLEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

// Bitwise OR operator overload for LEDColor to allow combining multiple LEDs
inline CPortExpRem::LEDColor operator|(CPortExpRem::LEDColor a, CPortExpRem::LEDColor b) {
    return static_cast<CPortExpRem::LEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}