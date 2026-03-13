#include "mcp.h"

CMCPLOCAL::CMCPLOCAL() {
    pMCP = new MCP23017(MCP_ADDRESS, 99); // I2C address 0x20, reset pin not used
    pMCP->Init();
    pMCP->setPortMode(0xFF, B); // Set all pins of port B as outputs
}

CMCPLOCAL::~CMCPLOCAL() {
    delete pMCP;
    pMCP = nullptr;
}

void CMCPLOCAL::setColor(LEDColor tLEDColor, bool boState) {
    uint8_t currentState = pMCP->getPort(B); // Read the current state of port B
    
    if (boState) {
        currentState |= tLEDColor; // Set the specified LED color bits
    } else {
        currentState &= ~tLEDColor; // Clear the specified LED color bits
    }
    pMCP->setPort(currentState, B); // Set the specified LED color on port B
}