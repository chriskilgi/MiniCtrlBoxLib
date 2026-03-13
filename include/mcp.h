#include <MCP23017.h>

#define MCP_ADDRESS 0x20

enum LEDColor {
    ALL = 0b00111111,
    RED0 = 0b00000001,
    GREEN0 = 0b00000010,
    BLUE0 = 0b00000100,
    RED1 = 0b00001000,
    GREEN1 = 0b00010000,
    BLUE1 = 0b00100000
};

inline LEDColor operator|(LEDColor a, LEDColor b) {
    return static_cast<LEDColor>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

class CMCPLOCAL {
public:
    CMCPLOCAL();
    ~CMCPLOCAL();

    void setColor(LEDColor tLEDColor, bool boState);

private:
    MCP23017 *pMCP;
};

