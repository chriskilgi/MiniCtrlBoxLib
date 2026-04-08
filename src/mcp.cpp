#include <glo.h>
#include <mcp.h>

// The CMCP class provides a base class for controlling MCP23017 I/O expanders,
// which are used to control the RGB LEDs and the switch LEDs in the MiniCtrlBox project. 
// It initializes the MCP23017 and provides a common interface for derived
// classes to control specific functionalities of the MCP23017.
CMCP::CMCP(uint8_t ui8MCPAddress) {
    pMCP = nullptr; // Initialize the MCP pointer to nullptr
    this->ui8MCPAddress = ui8MCPAddress; // Store the I2C address for later use in derived classes
}

CMCP::~CMCP() {
    delete pMCP;
    pMCP = nullptr;
}

bool CMCP::isPresent() {
    return pMCP != nullptr; // The MCP is considered present if the pointer is not null
}

/*----------------------------------------------------------------------------------------------*/
// The CPortExpLoc class inherits from CMCP and provides specific functionality for controlling
// the RGB LEDs connected to the MCP23017 on the Mainboard
CPortExpLoc::CPortExpLoc(uint8_t ui8MCPAddress) : CMCP(ui8MCPAddress) {
    // The constructor of CMCP will initialize the MCP23017 instance with the specified address
}

CPortExpLoc::~CPortExpLoc() {
    // The destructor of the parent class CMCP will clean up the MCP23017 instance
}

// The begin function initializes the MCP23017 and sets the pin modes for the RGB LEDs
bool CPortExpLoc::onBegin() {
    if (!gloIsI2CDevicePresent(ui8MCPAddress)) { // Check if the MCP23017 device is present at the specified I2C address
        return false; // If the device is not present, exit the function (pMCP will remain nullptr to indicate that the device is not available)
    } else {
        pMCP = new MCP23017(ui8MCPAddress, 99); // Create an instance of the MCP23017 with the specified I2C address and reset pin not used
    }
    
    pMCP->Init(); // Initialize the MCP23017
    delay(10); // Short delay to ensure the MCP23017 is ready after initialization
    pMCP->setPortMode(0xFF, B); // Set all pins of port B as outputs (for RGB LEDs)
    pMCP->setPortMode(0xFF, A); // Set all pins of port A as outputs (as they are not used for the RGB LEDs, we can set them as outputs to avoid floating inputs)
    pMCP->setPort(0x01, A); 
    return true;
}

// Function to set the state of the RGB LEDs on the Mainboard based on the specified color and state
void CPortExpLoc::setColor(RGBLEDColor tLEDColor, bool boState) {
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

// Overloaded function to set the state of a specific LED color on the Mainboard based on the color index (0-5) and state
void CPortExpLoc::setColor(uint8_t ui8Color, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (ui8Color >= 0 && ui8Color <= 5) {
        pMCP->setPin(ui8Color, B, boState); // Set the specified LED color on port B
    }
}   

/*----------------------------------------------------------------------------------------------*/
// The CPortExpRem class inherits from CMCP and provides specific functionality for controlling
// the LEDs and the switches connected to the MCP23017 on the SwitchLEDBoard
CPortExpRem::CPortExpRem(uint8_t ui8MCPAddress) : CMCP(ui8MCPAddress) {
    // The constructor of CMCP will initialize the MCP23017 instance with the specified address
}

CPortExpRem::~CPortExpRem() {
    // The destructor of CMCP will clean up the MCP23017 instance
}

// The begin function initializes the MCP23017 and sets the pin modes for the LEDs and switches
bool CPortExpRem::onBegin() {
    if (!gloIsI2CDevicePresent(ui8MCPAddress)) { // Check if the MCP23017 device is present at the specified I2C address
        return false; // If the device is not present, exit the function (pMCP will remain nullptr to indicate that the device is not available)
    } else {
        pMCP = new MCP23017(ui8MCPAddress, 99); // Create an instance of the MCP23017 with the specified I2C address and reset pin not used
    }

    pMCP->Init(); // Initialize the MCP23017
    delay(10); // Short delay to ensure the MCP23017 is ready after initialization
    pMCP->setPortMode(0x00, B); // Set all pins of port B as inputs (for switches)
    pMCP->setPortMode(0xFF, A); // Set all pins of port A as outputs (for LEDs)
    return true;
}

// Function to set the state of the LEDs on the SwitchLEDBoard based on the specified LED color and state
void CPortExpRem::setLED(LEDColor tLEDColor, bool boState) {
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

// Overloaded function to set the state of a specific LED on the SwitchLEDBoard based on the LED index (0-7) and state
void CPortExpRem::setLED(uint8_t ui8LED, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (ui8LED >= 0 && ui8LED <= 7) {
        pMCP->setPin(ui8LED, A, boState); // Set the specified LED on port A
    }
}   

// Function to set the state of all LEDs on the SwitchLEDBoard based on a port state byte
void CPortExpRem::setLEDPort(uint8_t ui8PortState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    pMCP->setPort(ui8PortState, A); // Set the specified LED color on port A
}

// Function to read the state of the switches on the SwitchLEDBoard
uint8_t CPortExpRem::getSwitchState() {
    if(pMCP == nullptr) {
        return 0; // If the MCP instance is not initialized, exit the function
    }
    return pMCP->getPort(B); // Read and return the state of the switches from port B
}

bool CPortExpRem::getSwitchState(uint8_t ui8SwitchNo) {
    if(pMCP == nullptr) {
        return false; // If the MCP instance is not initialized, exit the function
    }
    if (ui8SwitchNo >= 0 && ui8SwitchNo <= 7) {
        return pMCP->getPin(ui8SwitchNo, B); // Read and return the state of the specified switch from port B
    }
    return false;
}