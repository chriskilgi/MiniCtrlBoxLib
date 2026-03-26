#include <mcp.h>

// The CMCP class provides a base class for controlling MCP23017 I/O expanders,
// which are used to control the RGB LEDs and the switch LEDs in the MiniCtrlBox project. 
// It initializes the MCP23017 and provides a common interface for derived
// classes to control specific functionalities of the MCP23017.
CMCP::CMCP(uint8_t mcpAddress) {
    pMCP = nullptr; // Initialize the MCP pointer to nullptr
    pMCP = new MCP23017(mcpAddress, 99); // Create an instance of the MCP23017 with the specified I2C address and reset pin not used
}

CMCP::~CMCP() {
    delete pMCP;
    pMCP = nullptr;
}

/*----------------------------------------------------------------------------------------------*/
// The CMCPLOCAL class inherits from CMCP and provides specific functionality for controlling
// the RGB LEDs connected to the MCP23017 on the Mainboard
CMCPLOCAL::CMCPLOCAL(uint8_t mcpAddress) : CMCP(mcpAddress) {
    // The constructor of CMCP will initialize the MCP23017 instance with the specified address
}

CMCPLOCAL::~CMCPLOCAL() {
    // The destructor of CMCP will clean up the MCP23017 instance
}

void CMCPLOCAL::begin() {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }

    pMCP->Init(); // Initialize the MCP23017
    delay(10); // Short delay to ensure the MCP23017 is ready after initialization
    pMCP->setPortMode(0xFF, B); // Set all pins of port B as outputs (for RGB LEDs)
    pMCP->setPortMode(0xFF, A); // Set all pins of port A as outputs (as they are not used for the RGB LEDs, we can set them as outputs to avoid floating inputs)
}


void CMCPLOCAL::setColor(RGBLEDColor tLEDColor, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }

    uint8_t currentState = pMCP->getPort(B); // Read the current state of port B
    
    if (boState) {
        currentState |= tLEDColor; // Set the specified LED color bits
    } else {
        currentState &= ~tLEDColor; // Clear the specified LED color bits
    }
    pMCP->setPort(currentState, B); // Set the specified LED color on port B
}

/*----------------------------------------------------------------------------------------------*/
// The CMCPSLB class inherits from CMCP and provides specific functionality for controlling
// the LEDs and the switches connected to the MCP23017 on the SwitchLEDBoard
CMCPSLB::CMCPSLB(uint8_t mcpAddress) : CMCP(mcpAddress) {
    // The constructor of CMCP will initialize the MCP23017 instance with the specified address
}

CMCPSLB::~CMCPSLB() {
    // The destructor of CMCP will clean up the MCP23017 instance
}

void CMCPSLB::begin() {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }

    pMCP->Init(); // Initialize the MCP23017
    delay(10); // Short delay to ensure the MCP23017 is ready after initialization
    pMCP->setPortMode(0x00, B); // Set all pins of port B as inputs (for switches)
    pMCP->setPortMode(0xFF, A); // Set all pins of port A as outputs (for LEDs)
}

void CMCPSLB::setLED(LEDColor tLEDColor, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }

    uint8_t currentState = pMCP->getPort(A); // Read the current state of port A
    
    if (boState) {
        currentState |= tLEDColor; // Set the specified LED color bits
    } else {
        currentState &= ~tLEDColor; // Clear the specified LED color bits
    }
    pMCP->setPort(currentState, A); // Set the specified LED color on port A
}