#include <dsp.h>

COLed::COLed(TwoWire& wire, int8_t rst) : display(WIDTH, HEIGHT, &wire, rst) {

}

// Initialising the display and returning true if successful, false otherwise
bool COLed::begin() {
        if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) {
            boIsPresent = false;
            return false;
        }
        boIsPresent = true;
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.display();
        return true;
}

// Check if the display is present and initialized successfully
bool COLed::isPresent() {
     return boIsPresent;
}   

// Clears the content of a specific line (starting at 0)
void COLed::clearLine(uint8_t row) {
    if (row >= ROWS) return;

    display.fillRect(
        0,
        row * CHAR_H,
        WIDTH,
        CHAR_H,
        SSD1306_BLACK
    );
}

// Prints text at a specific row and column (starting at 0)
void COLed::printAt(uint8_t row, uint8_t col, const char* text) {
    if (row >= ROWS || col >= COLS) return;

    // Clear the line before printing
    clearLine(row);

    // Set cursor position based on row and column
    display.setCursor(col * CHAR_W, row * CHAR_H);

    // Print the text
    display.print(text);

    // Update the display to show the changes
    display.display();
}

// Override whole line (automatically starts at column 0)
void COLed::printLine(uint8_t row, const char* text) {
    printAt(row, 0, text);
}

// Deletes the entire display content
void COLed::clear() {
    display.clearDisplay();
    display.display();
}

// Prints formatted text to a specific position on the display
// Example usage: printfAt(2, 5, "Temp: %.1f C", temperature);
void COLed::printfAt(uint8_t row, uint8_t col, const char* fmt, ...) {
    if (row >= ROWS || col >= COLS) return;

    char buffer[64];   // oder größer, je nach Bedarf

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    printAt(row, col, buffer);
}

// Prints formatted text to a specific line, starting at column 0
// Example usage: printfLine(2, "Temperature: %.1f C", temperature);
void COLed::printfLine(uint8_t row, const char* fmt, ...) {
    char buffer[64];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    printLine(row, buffer);
}