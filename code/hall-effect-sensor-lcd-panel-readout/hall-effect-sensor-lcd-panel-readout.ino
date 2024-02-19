#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
const int rs = 16, en = 10, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
volatile int magnetCounter = 0; // Counter for magnetic field change detections

void setup() {
  lcd.begin(16, 2); // Specify the dimensions of the LCD
  pinMode(2, INPUT_PULLUP); // Assuming active-low sensor. Use INPUT if active-high.
  
  // Attach an interrupt to the Hall Effect sensor pin (pin 2)
  attachInterrupt(digitalPinToInterrupt(2), magnetDetectedISR, CHANGE);
}

void loop() {
  static int lastDisplayCount = -1; // Store the last displayed count for comparison
  noInterrupts(); // Disable interrupts while accessing the shared variable
  int displayCount = magnetCounter; // Copy the counter
  interrupts(); // Re-enable interrupts
  
  // Check if the count has changed or it's the first update
  if (displayCount != lastDisplayCount) {
    lastDisplayCount = displayCount; // Update last displayed count
    
    // Construct the display strings
    String passesStr = "Passes: " + String(displayCount);
    String stateStr = "State:  " + String((displayCount % 2 == 0) ? "closed" : "open");
    
    // Clear previous content by overwriting with spaces
    lcd.setCursor(0, 0);
    lcd.print(passesStr + "                "); // Ensure to overwrite the entire line
    lcd.setCursor(0, 1);
    lcd.print(stateStr + "                "); // Ensure to overwrite the entire line
  }
}

// Interrupt service routine for detecting magnet
void magnetDetectedISR() {
  magnetCounter++;
}
