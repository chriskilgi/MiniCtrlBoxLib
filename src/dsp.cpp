#include <dsp.h>

COLed::COLed(TwoWire& wire, int8_t rst) : display(WIDTH, HEIGHT, &wire, rst) {

}

// Initialisierung des Displays
bool COLed::begin() {
        if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) {
            return false;
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.display();
        return true;
}

// Zeile löschen
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

// Text an Zeile/Spalte ausgeben
void COLed::printAt(uint8_t row, uint8_t col, const char* text) {
    if (row >= ROWS || col >= COLS) return;

    // Zeile vorher löschen
    clearLine(row);

    // Cursor setzen
    display.setCursor(col * CHAR_W, row * CHAR_H);

    // Text ausgeben
    display.print(text);

    // Aktualisieren
    display.display();
}

// Ganze Zeile überschreiben (automatisch)
void COLed::printLine(uint8_t row, const char* text) {
    printAt(row, 0, text);
}

// Komplettes Display löschen
void COLed::clear() {
    display.clearDisplay();
    display.display();
}
