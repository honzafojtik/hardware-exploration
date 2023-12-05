#include "DHT.h"
#include <LiquidCrystal.h>
#define DHT22_PIN 2

DHT dht22(DHT22_PIN, DHT22);

const int rs = 16, en = 10, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  dht22.begin();
  lcd.begin(16, 2);
}

void loop() {
  delay(2000);

  float tempC = dht22.readTemperature();

  if (isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed to read from DHT22 sensor!");
    lcd.setCursor(0, 1);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.setCursor(0, 1);
    lcd.print(tempC);
    lcd.print(" C");
  }

  delay(1000);
}
