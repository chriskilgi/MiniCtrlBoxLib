#include <dsp.h>

namespace nspMiniCtrlBox {
COLed::COLed(TwoWire& wire, int8_t rst) 
    : display(WIDTH, HEIGHT, &wire, rst), textSize(1) {
}

// Initialising the display and returning true if successful, false otherwise
bool COLed::begin() {
    Wire.begin();
    if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) {
        boIsPresent = false;
        return false;
    }

    boIsPresent = true;
    display.clearDisplay();
    setTextSize(1);   // Standard: kleine Schrift
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.display();
    return true;
}

bool COLed::isPresent() {
    return boIsPresent;
}

// Set the text size (1 = small, 2 = large)
void COLed::setTextSize(uint8_t ui8Size) {
    if (ui8Size < 1) ui8Size = 1;
    if (ui8Size > 3) ui8Size = 3;   // optional: limit to 3

    textSize = ui8Size;
    display.setTextSize(textSize);

    // Update character dimensions
    charW = BASE_CHAR_W * textSize;
    charH = BASE_CHAR_H * textSize;

    rows = HEIGHT / charH;
    cols = WIDTH / charW;
}

void COLed::clearLine(uint8_t ui8Row) {
    if (ui8Row >= rows) return;

    display.fillRect(
        0,
        ui8Row * charH,
        WIDTH,
        charH,
        SSD1306_BLACK
    );
}

void COLed::printAt(uint8_t ui8Row, uint8_t ui8Col, const char* pcText) {
    if (ui8Row >= rows || ui8Col >= cols) return;

    clearLine(ui8Row);

    display.setCursor(ui8Col * charW, ui8Row * charH);
    display.print(pcText);
    display.display();
}

void COLed::printLine(uint8_t ui8Row, const char* pcText) {
    printAt(ui8Row, 0, pcText);
}

void COLed::clear() {
    display.clearDisplay();
    display.display();
}

void COLed::printfAt(uint8_t ui8Row, uint8_t ui8Col, const char* pcFormattedText, ...) {
    if (ui8Row >= rows || ui8Col >= cols) return;

    char buffer[64];

    va_list args;
    va_start(args, pcFormattedText);
    vsnprintf(buffer, sizeof(buffer), pcFormattedText, args);
    va_end(args);

    printAt(ui8Row, ui8Col, buffer);
}

void COLed::printfLine(uint8_t ui8Row, const char* pcFormattedText, ...) {
    if (ui8Row >= rows) return;

    char buffer[64];

    va_list args;
    va_start(args, pcFormattedText);
    vsnprintf(buffer, sizeof(buffer), pcFormattedText, args);
    va_end(args);

    printLine(ui8Row, buffer);
}
} // namespace nspMiniCtrlBox
