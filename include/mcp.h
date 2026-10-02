// mcp.h
// This file is part of the MiniCtrlBoxLib - A library for the MiniCtrlBox project.
// Copyright (c) 2026 by Christoph Kilgenstein. All rights reserved.
// Headerfile for mcp.cpp (MCP23017)
// Neccessary for the Mainboard and SwitchLEDBoard of the MiniCtrlBox project.

#pragma once
#include <MCP23017.h>

namespace nspMiniCtrlBox {
#define MCP_LOCAL_ADDRESS 0x20  // Address for the MCP23017 on the Mainboard
#define MCP_SLB_ADDRESS 0x21    // Address for the MCP23017 on the SwitchLEDBoard
#define MCP_OOPExp_ADDRESS 0x21 // Address for the MCP23017 on the OOPExpBoard

#define PIN_INT_MCP_SLB_B 0     // Interrupt pin for the MCP23017 on the SwitchLEDBoard
#define PIN_INT_MCP_OOPEXP_B 3  // Interrupt pin for the MCP23017 on the OOPExpBoard

#define MINICTRLBOX_VERSION_GLOB MINICTRLBOX_VERSION
// The CMCP class provides a common interface for controlling MCP23017 port expanders, with derived classes for specific boards (Mainboard and SwitchLEDBoard).
class CMCP {
    public:
        // The Destructor has to be public, so that the derived objects can be deleted properly,
        // since the destructor of the derived class will call the destructor of the base class automatically
        ~CMCP();
        bool begin(); // Initialize the MCP23017 and set the pin modes for ports 
        bool isPresent(); // Check if the MCP23017 is present on the I2C bus

    protected:
        CMCP(uint8_t ui8MCPAddress); // The constructor is protected to prevent direct instantiation of the base class

        uint8_t ui8MCPAddress; // I2C address of the MCP23017
        MCP23017 *pMCP; // Pointer to the MCP23017 instance, initialized in derived classes if the device is present

        uint8_t ui8PortMaskA; // Mask for port A of the MCP23017 on the Mainboard respectively the SwitchLEDBoard respectively the OOPExpBoard
        uint8_t ui8PortMaskB; // Mask for port B of the MCP23017 on the Mainboard respectively the SwitchLEDBoard respectively the OOPExpBoard        
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

        // Function to set the state of the RGB LEDs based on the specified color and state
        void setColor(RGBLEDColor tLEDColor, bool boState);
        // Overloaded function to set the state of the RGB LEDs based on a color index (0-5) and state
        void setColor(uint8_t ui8Color, bool boState);
    protected:

};

// The CPortExpRem class inherits from CMCP and provides specific functionality for controlling
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
        
        void setLED(LEDColor tLEDColor, bool boState);
        void setLED(uint8_t ui8LED, bool boState);
        void setLEDPort(uint8_t ui8PortState); // Function to set the state of all LEDs on the SwitchLEDBoard based on a port state byte
        void setLEDPortRemain(uint8_t ui8PortState); // Function to set the state of all LEDs on the SwitchLEDBoard based on a port state byte, while keeping the current state of the other LEDs unchanged
        void resetLEDPortRemain(uint8_t ui8PortState); // Function to reset the state of all LEDs on the SwitchLEDBoard based on a port state byte, while keeping the current state of the other LEDs unchanged
        void toggleLEDPortRemain(uint8_t ui8PortState); // Function to toggle the state of all LEDs on the SwitchLEDBoard based on a port state byte, while keeping the current state of the other LEDs unchanged

        uint8_t getSwitchState(); // Function to read the state of the switches on the SwitchLEDBoard
        bool getSwitchState(uint8_t ui8SwitchNo); // Overloaded function to check if specific switches are pressed based on a switch index (0-7)
        uint8_t getLEDPortState(); // Function to read the state of all LEDs on the SwitchLEDBoard as a port state byte

        void setInterruptMask(uint8_t ui8Mask); // Function to set the interrupt mask for the switches on the SwitchLEDBoard
        void enableInterrupts(void (*callbackFunction)(void)); // Function to enable interrupts for the switches on the SwitchLEDBoard
        void disableInterrupts(); // Function to disable interrupts for the switches on the SwitchLEDBoard
        uint8_t getInterruptFlag(void); // Function to read the interrupt flag for the switches on the SwitchLEDBoard
    protected:
        uint8_t ui8FilterMaskSwitchState; // Mask to filter the switch state in the derived class CPortExpOOPExp, since the OOPExpBoard only has 4 switches (SW1-SW4) and the other 4 bits are not relevant for the OOPExpBoard
        uint8_t ui8MaxSwitchNo; // Maximum switch number for the derived class CPortExpOOPExp, since the OOPExpBoard only has 4 switches (SW1-SW4) and the other 4 bits are not relevant for the OOPExpBoard
        uint8_t ui8InterruptPin; // Interrupt pin, since the OOPExpBoard has a different interrupt pin than the SwitchLEDBoard
};

// The CPortExpOOPExp class inherits from CPortExpRem and provides specific functionality for controlling
class CPortExpOOPExp : public CPortExpRem {
    public:
        // Konstanten für die LEDs auf dem OOPExpBoard
        enum LEDColor : uint8_t {
            ALLLEDS = 0b11111111,
            LEDFG0 = 0b00000001,
            LEDFG1 = 0b00000010,
            LEDFG2 = 0b00000100,
            LEDFG3 = 0b00001000,
            LEDSW1 = 0b00010000,
            LEDSW2 = 0b00100000,
            LEDSW3 = 0b01000000,
            LEDSW4 = 0b10000000
        };    

        CPortExpOOPExp(uint8_t ui8MCPAddress = MCP_OOPExp_ADDRESS); // Default I2C address for the OOPExpBoard
        
        void setLED(LEDColor tLEDColor, bool boState);
};

// Bitwise OR operator overload for RGBLEDColor combining LED colors
inline CPortExpLoc::RGBLEDColor operator|(CPortExpLoc::RGBLEDColor a, CPortExpLoc::RGBLEDColor b) {
    return static_cast<CPortExpLoc::RGBLEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}


// Bitwise OR operator overload for LEDColor to allow combining multiple LEDs
inline CPortExpRem::LEDColor operator|(CPortExpRem::LEDColor a, CPortExpRem::LEDColor b) {
    return static_cast<CPortExpRem::LEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
} // namespace nspMiniCtrlBox   