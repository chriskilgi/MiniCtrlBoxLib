#include <glo.h>
#include <mcp.h>

namespace nspMiniCtrlBox {
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

// The begin function initializes the MCP23017 and sets the pin modes for the RGB LEDs
bool CMCP::begin() {
    Wire.begin(); // Initialize I2C communication

    if (!gloIsI2CDevicePresent(ui8MCPAddress)) { // Check if the MCP23017 device is present at the specified I2C address
        return false; // If the device is not present, exit the function (pMCP will remain nullptr to indicate that the device is not available)
    } else if (pMCP == nullptr) {
        pMCP = new MCP23017(ui8MCPAddress, 99); // Create an instance of the MCP23017 with the specified I2C address and reset pin not used
    }
    
    if (pMCP == nullptr) {
        return false; // If the MCP instance could not be created, exit the function
    }

    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        pMCP->Init(); // Initialize the MCP23017
        delay(10); // Short delay to ensure the MCP23017 is ready after initialization
        pMCP->setPortMode(ui8PortMaskA, A); // Set pins of port A as set in constructor of derived classes (for LEDs or switches)
        pMCP->setPortMode(ui8PortMaskB, B); // Set pins of port B as set in constructor of derived classes (for LEDs or switches)
        // Set all OUTPUT pins of the MCP23017 to LOW to ensure that all LEDs are off at startup
        pMCP->setPort(~ui8PortMaskA, A); // Set all OUTPUT pins of port A to LOW
        pMCP->setPort(~ui8PortMaskB, B); // Set all OUTPUT pins of port B to LOW
        xSemaphoreGive(xMutexI2C_g);
    }
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// The CKeyPad class inherits from CMCP and provides specific functionality for controlling

// 
CKeyPad::CKeyPad(uint8_t ui8MCPAddress) : CMCP(ui8MCPAddress) {
    // The constructor of CMCP will initialize the MCP23017 instance with the specified address
    ui8PortMaskA = 0xF0; // Mask for port A of the MCP23017 on the KeyPad-Board: lower 4 bits are inputs (columns), upper 4 bits are outputs (LEDs and beeper)
    ui8PortMaskB = 0xF0; // Mask for port B of the MCP23017 on the KeyPad-Board: lower 4 bits are inputs (rows), upper 4 bits are not used

    ui8InterruptPinPortA = PIN_INT_MCP_KEYPAD_A; // Interrupt pin for Port A of the MCP23017 on the KeyPad-Board
    ui8InterruptPinPortB = PIN_INT_MCP_KEYPAD_B; // Interrupt pin for Port B of the MCP23017 on the KeyPad-Board

    pinMode(ui8InterruptPinPortA, INPUT_PULLUP); // Set the interrupt pin for Port A as an input
    pinMode(ui8InterruptPinPortB, INPUT_PULLUP); // Set the interrupt pin for Port B as an input

    ui8InterruptMaskPortA = 0x0F; // Interrupt mask for Port A of the MCP23017 on the KeyPad-Board: lower 4 bits (columns) will trigger interrupts
    ui8InterruptMaskPortB = 0x0F; // Interrupt mask for Port B of the MCP23017 on the KeyPad-Board: lower 4 bits (rows) will trigger interrupts
}

// Function to switch on/off the LEDs on the KeyPad-Board, which are connected to pins GPA5 and GPA6 of the MCP23017
// The function takes an ELEDBitmask to specify which LEDs to control and a boolean state to turn them on or off
void CKeyPad::setLED(ELEDBitmask LEDBitmask, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }    
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        if(boState) {
            pMCP->setPort(LEDBitmask, A); // Set the specified LEDs color on port A
        } else {
            pMCP->setPort(~LEDBitmask & 0b01100000, A); // Clear the specified LEDs color on port A
        }
        xSemaphoreGive(xMutexI2C_g);
    }    
}

// Function to switch on/off the LEDs on the KeyPad-Board, which are connected to pins GPA5 and GPA6 of the MCP23017
// The function takes an ELEDNumber to specify which LED to control and a boolean state to turn it on or off
void CKeyPad::setLED(ELEDNumber LEDNo, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }    
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t ui8LEDPin = (LEDNo == 0) ? 5 : (LEDNo == 1) ? 6 : 7; // Map LED number to the corresponding pin on port A
        pMCP->setPin(ui8LEDPin, A, boState); // Set the specified LED color on port A
        xSemaphoreGive(xMutexI2C_g);
    }    
}

// Function to switch on/off the beeper on the KeyPad-Board, which is connected to pin GPA7 of the MCP23017
void CKeyPad::setBeeper(bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }    
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        pMCP->setPin(PIN_BEEPER_KEYPAD, A, boState); // Set the beeper pin on port A
        xSemaphoreGive(xMutexI2C_g);
    }    
}

// Overloaded Function to switch on the beeper on the KeyPad-Board for a specified duration in milliseconds
void CKeyPad::beeper(unsigned short int usiMilliseconds) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }    
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        pMCP->setPin(PIN_BEEPER_KEYPAD, A, true);
        delay(usiMilliseconds);
        pMCP->setPin(PIN_BEEPER_KEYPAD, A, false);
        xSemaphoreGive(xMutexI2C_g);
    }    
}

// Because of an error in the pin assignment within the LM339 in EasyEDA the ouput level
// of the third comparator (Pin 14) has to be inverted (this output pin is wired to GPA2 of the MCP23017)
uint8_t CKeyPad::correctWrongPinLevel(uint8_t ui8Key) {
    uint8_t ui8KeyCorrected;

    ui8KeyCorrected = ui8Key ^ 0b00000100;  // Toggle the third bit
    return ui8KeyCorrected; 
}

// Function to read the key pressed on the KeyPad-Board, returns a value from 0 to 15 for keys 0-15, or 0xFF if no key is pressed
uint8_t CKeyPad::getKey(void) {
    if(pMCP == nullptr) {
        return 0xFA; // If the MCP instance is not initialized, exit the function with error code
    }    
    
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t ui8Col = correctWrongPinLevel(pMCP->getPort(A)); // Read the column value from port A and correct the pin level due to the hardware error
        uint8_t ui8Row = pMCP->getPort(B);
        xSemaphoreGive(xMutexI2C_g);    // I2C-Bus access is finished, so the mutex can be released

        ui8Col &= 0x0F; // Mask to get the lower 4 bits of port A (columns)
        ui8Row &= 0x0F; // Mask to get the lower 4 bits of port B (rows)

        // Check if any key is pressed by checking if the combined value of columns and rows is not 0xFF
        // In passive mode, if no key is pressed, the columns and rows will both read as 0x0F (active low), resulting in a combined value of 0xFF
        bool boKeyPressed = (((ui8Col << 4) | ui8Row) == 0xFF) ? false : true;

        // Because the locic is active low, all bits have to be inverted
        ui8Col = ~ui8Col & 0x0F; // Invert the column bits and mask to get the active column
        ui8Row = ~ui8Row & 0x0F; // Invert the row bits and mask to get the active row

        ui8Col = log2(ui8Col); // Get the column value (0-3) from port A
        ui8Row = log2(ui8Row); // Get the row value (0-3) from port B

        uint8_t ui8Key = ui8Col + 4 * ui8Row; // Calculate the key value based on the row and column values

        return (boKeyPressed) ? ui8Key : 0xFF;
    }      
    return 0xFB; // Return error code if the mutex could not be taken
}

// Function to read the key pressed on the KeyPad-Board, returns the character for keys 0-15 printed on the keypad surface, or 0xFF if no key is pressed
unsigned char CKeyPad::getKeyChar(void) {
    uint8_t ui8Key = getKey(); // Get the key value (0-15) or 0xFF if no key is pressed
    if (ui8Key == 0xFF || ui8Key == 0xFA || ui8Key == 0xFB) { // Check if no key is pressed or if there was an error
        return ui8Key; // Return 0xFF if no key is pressed, or error codes 0xFA or 0xFB if there was an error
    }
    return ucKeyPadConversionTable[ui8Key]; // Return the corresponding character for the key value (0-9, A-F)
}

/* Interrupt functionality of class CKeyPad */
// Function to set the interrupt mask for the KeyPad-Board, which determines which keys will trigger an interrupt when pressed
void CKeyPad::setInterruptMask(void) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        pMCP->setInterruptOnChangePort(ui8InterruptMaskPortA, A);
        pMCP->setInterruptOnChangePort(ui8InterruptMaskPortB, B);
        xSemaphoreGive(xMutexI2C_g);
    }
}

// Function to enable interrupts for the switches on the KeyPad-Board, allowing the system to respond to key presses without polling
// The callbackFunction must have the form void IRAM_ATTR callbackFunction(void) and will be called when an interrupt occurs
void CKeyPad::enableInterrupts(void (*callbackFunction)(void)) {
    setInterruptMask(); // Set the interrupt mask for the KeyPad-Board
    attachInterrupt(digitalPinToInterrupt(ui8InterruptPinPortA), callbackFunction, FALLING);
    attachInterrupt(digitalPinToInterrupt(ui8InterruptPinPortB), callbackFunction, FALLING);
    freeInterrupts(); // Ensure that the interrupt flag is cleared in the beginning, so that the first interrupt can be detected correctly
}

// Function to disable interrupts for the switches on the SwitchLEDBoard
void CKeyPad::disableInterrupts() {
    detachInterrupt(digitalPinToInterrupt(ui8InterruptPinPortA));
    detachInterrupt(digitalPinToInterrupt(ui8InterruptPinPortB));
}

// Function to free new interrupts and returns the key value (0-15) for the pressed key, or 0xFF if no key is pressed
uint8_t CKeyPad::freeInterrupts() {
    if(pMCP == nullptr) {
        return 0; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t ui8CapA = pMCP->getIntCap(A); // Clear the interrupt flag for port A to allow further interrupts
        uint8_t ui8CapB = pMCP->getIntCap(B); // Clear the interrupt flag for port B to allow further interrupts
         xSemaphoreGive(xMutexI2C_g); // I2C-Bus access is finished, so the mutex can be released

        ui8CapA = correctWrongPinLevel(ui8CapA) & 0x0F; // Correct the pin level for port A due to the hardware error and mask to get the lower 4 bits
        ui8CapB = ui8CapB & 0x0F; // Mask to get the lower 4 bits of port B
        ui8CapA = ~ui8CapA & 0x0F; // Invert the bits of the captured value for port A to match the active low logic
        ui8CapB = ~ui8CapB & 0x0F; // Invert the bits of the captured value for port B to match the active low logic
        ui8CapA = log2(ui8CapA); // Get the column value (0-3) from port A
        ui8CapB = log2(ui8CapB); // Get the row value (0-3) from port B

        return (ui8CapA + 4 * ui8CapB); // Return the interrupt flag
    }
    return 0; // Return 0 if the MCP instance is not initialized or if the mutex could not be taken
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// The CPortExpLoc class inherits from CMCP and provides specific functionality for controlling
// the RGB LEDs connected to the MCP23017 on the Mainboard
CPortExpLoc::CPortExpLoc(uint8_t ui8MCPAddress) : CMCP(ui8MCPAddress) {
    // The constructor of CMCP will initialize the MCP23017 instance with the specified address
    ui8PortMaskA = 0xFF; // Mask for port A of the MCP23017 on the Mainboard, all pins are outputs
    ui8PortMaskB = 0xFF; // Mask for port B of the MCP23017 on the Mainboard, all pins are outputs
}

// Function to set the state of the RGB LEDs on the Mainboard based on the specified color and state
void CPortExpLoc::setColor(RGBLEDColor tLEDColor, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }

    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t currentState = pMCP->getPort(B); // Read the current state of port B
        
        if (boState) {
            currentState |= tLEDColor; // Set the specified LED color bits
        } else {
            currentState &= ~tLEDColor; // Clear the specified LED color bits
        }
        pMCP->setPort(currentState, B); // Set the specified LED color on port B
        xSemaphoreGive(xMutexI2C_g);
    }
}

// Overloaded function to set the state of a specific LED color on the Mainboard based on the color index (0-5) and state
void CPortExpLoc::setColor(uint8_t ui8Color, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        if (ui8Color >= 0 && ui8Color <= 5) {
            pMCP->setPin(ui8Color, B, boState); // Set the specified LED color on port B
        }
        xSemaphoreGive(xMutexI2C_g);
    }
}   

/*----------------------------------------------------------------------------------------------*/
// The CPortExpRem class inherits from CMCP and provides specific functionality for controlling
// the LEDs and the switches connected to the MCP23017 on the SwitchLEDBoard
CPortExpRem::CPortExpRem(uint8_t ui8MCPAddress) : CMCP(ui8MCPAddress) {
    // The constructor of CMCP will initialize the MCP23017 instance with the specified address

    ui8PortMaskA = 0xFF; // Set all pins of port A as outputs (for LEDs) 
    ui8PortMaskB = 0x00; // Set all pins of port B as inputs (for switches)   
    
    ui8FilterMaskSwitchState = 0xFF; // Default filter mask for the switch state (all bits are relevant)
    ui8MaxSwitchNo = 7; // Default maximum switch number (0-7 for SwitchLEDBoard)
    ui8InterruptPin = PIN_INT_MCP_SLB_B; // Set the interrupt pin for the SwitchLEDBoard

    // Set the interrupt pin for the MCP23017 on the SwitchLEDBoard as an input
    pinMode(ui8InterruptPin, INPUT);     
}

// Function to set the state of the LEDs on the SwitchLEDBoard based on the specified LED color and state
void CPortExpRem::setLED(LEDColor tLEDColor, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }

    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t currentState = pMCP->getPort(A); // Read the current state of port A
        
        if (boState) {
            currentState |= tLEDColor; // Set the specified LED color bits
        } else {
            currentState &= ~tLEDColor; // Clear the specified LED color bits
        }
        pMCP->setPort(currentState, A); // Set the specified LED color on port A
        xSemaphoreGive(xMutexI2C_g);
    }
}

// Overloaded function to set the state of a specific LED on the SwitchLEDBoard based on the LED index (0-7) and state
void CPortExpRem::setLED(uint8_t ui8LED, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        if (ui8LED >= 0 && ui8LED <= 7) {
            pMCP->setPin(ui8LED, A, boState); // Set the specified LED on port A
        }
        xSemaphoreGive(xMutexI2C_g);
    }
}   

// Function to set the state of all LEDs on the SwitchLEDBoard based on a port state byte
void CPortExpRem::setLEDPort(uint8_t ui8PortState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        pMCP->setPort(ui8PortState, A); // Set the specified LED color on port A
        xSemaphoreGive(xMutexI2C_g);
    }
}

// Function to set the state of all LEDs on the SwitchLEDBoard based on a port state byte, while keeping the current state of the other LEDs unchanged
void CPortExpRem::setLEDPortRemain(uint8_t ui8PortState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t currentState = pMCP->getPort(A); // Read the current state of port A
        currentState |= ui8PortState; // Set the specified LED color bits while keeping the current state of the other LEDs unchanged
        pMCP->setPort(currentState, A); // Set the specified LED color on port A
        xSemaphoreGive(xMutexI2C_g);
    }
}

// Function to reset the state of all LEDs on the SwitchLEDBoard based on a port state byte, while keeping the current state of the other LEDs unchanged
void CPortExpRem::resetLEDPortRemain(uint8_t ui8PortState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t currentState = pMCP->getPort(A); // Read the current state of port A
        currentState &= ~ui8PortState; // Clear the specified LED color bits while keeping the current state of the other LEDs unchanged
        pMCP->setPort(currentState, A); // Set the specified LED color on port A
        xSemaphoreGive(xMutexI2C_g);
    }
}

void CPortExpRem::toggleLEDPortRemain(uint8_t ui8PortState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t currentState = pMCP->getPort(A); // Read the current state of port A
        currentState ^= ui8PortState; // Toggle the specified LED color bits while keeping the current state of the other LEDs unchanged
        pMCP->setPort(currentState, A); // Set the specified LED color on port A
        xSemaphoreGive(xMutexI2C_g);
    }
}

// Function to read the state of the switches on the SwitchLEDBoard
uint8_t CPortExpRem::getSwitchState() {
    if(pMCP == nullptr) {
        return 0; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t switchState = pMCP->getPort(B); // Read the state of the switches from port B
        xSemaphoreGive(xMutexI2C_g);
        return switchState & ui8FilterMaskSwitchState; // Return the state of the switches
    }
    return 0; // Return 0 if the MCP instance is not initialized or if the mutex could not be taken
}

bool CPortExpRem::getSwitchState(uint8_t ui8SwitchNo) {
    if(pMCP == nullptr) {
        return false; // If the MCP instance is not initialized, exit the function
    }
    
    if (ui8SwitchNo >= 0 && ui8SwitchNo <= ui8MaxSwitchNo) { // Check if the switch number is valid (0-7 for SwitchLEDBoard, 0-3 for OOPExpBoard)
        if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
            bool switchState = pMCP->getPin(ui8SwitchNo, B); // Read the state of the specified switch from port B
            xSemaphoreGive(xMutexI2C_g);
            return switchState; // Return the state of the specified switch
        }
    }
    return false;
}

uint8_t CPortExpRem::getLEDPortState() {
    if(pMCP == nullptr) {
        return 0; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t ledState = pMCP->getPort(A); // Read the state of the LEDs from port A
        xSemaphoreGive(xMutexI2C_g);
        return ledState; // Return the state of the LEDs
    }
    return 0; // Return 0 if the MCP instance is not initialized or if the mutex could not be taken
}

// Function to set the interrupt mask for the switches on the SwitchLEDBoard
void CPortExpRem::setInterruptMask(uint8_t ui8Mask) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        pMCP->setInterruptOnChangePort(ui8Mask, B); // Set the interrupt mask for the switches on the SwitchLEDBoard
        xSemaphoreGive(xMutexI2C_g);
    }
}

// Function to enable interrupts for the switches on the SwitchLEDBoard
// The callbackFunction must have the form void IRAM_ATTR callbackFunction(void) and will be called when an interrupt occurs
void CPortExpRem::enableInterrupts(void (*callbackFunction)(void)) {
    attachInterrupt(digitalPinToInterrupt(ui8InterruptPin), callbackFunction, FALLING);
    getInterruptFlag(); // Ensure that the interrupt flag is cleared in the beginning, so that the first interrupt can be detected correctly
}

// Function to disable interrupts for the switches on the SwitchLEDBoard
void CPortExpRem::disableInterrupts() {
    detachInterrupt(digitalPinToInterrupt(ui8InterruptPin));
}

// Function to read the interrupt flag for the switches on the SwitchLEDBoard
// The interrupt flag indicates which switches have triggered an interrupt since the last read
uint8_t CPortExpRem::getInterruptFlag() {
    if(pMCP == nullptr) {
        return 0; // If the MCP instance is not initialized, exit the function
    }
    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t interruptFlag = pMCP->getIntFlag(B); // Read the interrupt flag for port B
        pMCP->getIntCap(B); // Clear the interrupt flag for port B to allow further interrupts
        xSemaphoreGive(xMutexI2C_g);
        return interruptFlag; // Return the interrupt flag
    }
    return 0; // Return 0 if the MCP instance is not initialized or if the mutex could not be taken
}

/* ***************************************************** End of class CPortExpRem ********************************************************** */

/*----------------------------------------------------------------------------------------------*/
// The CPortExpOOPExp class inherits from CPortExpRem and changes specific functionality for controlling
// the LEDs and the switches connected to the MCP23017 on the OOPExp-Board
CPortExpOOPExp::CPortExpOOPExp(uint8_t ui8MCPAddress) : CPortExpRem(ui8MCPAddress) {
    // The constructor of CMCP will initialize the MCP23017 instance with the specified address
    
    ui8PortMaskA = 0xFF; // Set all pins of port A as outputs (for LEDs)
    ui8PortMaskB = 0xF0; // Set the lower 4 pins of port B as inputs (for the four switches)

    ui8FilterMaskSwitchState = 0x0F; // Filter mask for the switch state (only the lower 4 bits are relevant for the OOPExpBoard)
    ui8MaxSwitchNo = 3; // Maximum switch number (0-3 for OOPExpBoard)
    ui8InterruptPin = PIN_INT_MCP_OOPEXP_B; // Set the interrupt pin for the OOPExpBoard

    // Set the interrupt pin for the MCP23017 on the OOPExpBoard as an input
    pinMode(ui8InterruptPin, INPUT);     
}


// Function to set the state of the LEDs on the OOPExpBoard based on the specified LED color and state
// The LED colors for the OOPExpBoard are different from the SwitchLEDBoard, so this function is overridden in the derived class CPortExpOOPExp
void CPortExpOOPExp::setLED(LEDColor tLEDColor, bool boState) {
    if(pMCP == nullptr) {
        return; // If the MCP instance is not initialized, exit the function
    }

    if (xSemaphoreTake(xMutexI2C_g, portMAX_DELAY)) {
        uint8_t currentState = pMCP->getPort(A); // Read the current state of port A
        
        if (boState) {
            currentState |= tLEDColor; // Set the specified LED color bits
        } else {
            currentState &= ~tLEDColor; // Clear the specified LED color bits
        }
        pMCP->setPort(currentState, A); // Set the specified LED color on port A
        xSemaphoreGive(xMutexI2C_g);
    }
}

/* ***************************************************** End of class CPortExpOOPExp ******************************************************* */
} // namespace nspMiniCtrlBox