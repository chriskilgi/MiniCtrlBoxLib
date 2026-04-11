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
void COLed::setTextSize(uint8_t size) {
    if (size < 1) size = 1;
    if (size > 3) size = 3;   // optional: limit to 3

    textSize = size;
    display.setTextSize(textSize);

    // Update character dimensions
    charW = BASE_CHAR_W * textSize;
    charH = BASE_CHAR_H * textSize;

    rows = HEIGHT / charH;
    cols = WIDTH / charW;
}

void COLed::clearLine(uint8_t row) {
    if (row >= rows) return;

    display.fillRect(
        0,
        row * charH,
        WIDTH,
        charH,
        SSD1306_BLACK
    );
}

void COLed::printAt(uint8_t row, uint8_t col, const char* text, uint8_t size) {
    if (size != 0) setTextSize(size);

    if (row >= rows || col >= cols) return;

    clearLine(row);

    display.setCursor(col * charW, row * charH);
    display.print(text);
    display.display();
}

void COLed::printLine(uint8_t row, const char* text, uint8_t size) {
    printAt(row, 0, text, size);
}

void COLed::clear() {
    display.clearDisplay();
    display.display();
}

void COLed::printfAt(uint8_t row, uint8_t col, const char* fmt, uint8_t size, ...) {
    if (row >= rows || col >= cols) return;

    char buffer[64];

    va_list args;
    va_start(args, size);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    printAt(row, col, buffer, size);
}

void COLed::printfLine(uint8_t row, const char* fmt, uint8_t size, ...) {
    if (row >= rows) return;

    char buffer[64];

    va_list args;
    va_start(args, size);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    printLine(row, buffer, size);
}
} // namespace nspMiniCtrlBox
