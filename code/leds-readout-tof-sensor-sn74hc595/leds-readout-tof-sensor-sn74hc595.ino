#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

const int SN74HC595_SER_PIN = A0;
const int SN74HC595_RCLK_PIN = A1;
const int SN74HC595_SRCLK_PIN = A2;
const int SN74HC95_OE_PIN = 5;

void setup()
{
  while (!Serial) {}
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(400000);

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);
  sensor.setROISize(4, 4);
  sensor.startContinuous(50);

  pinMode(SN74HC595_SER_PIN, OUTPUT);
  pinMode(SN74HC595_RCLK_PIN, OUTPUT);
  pinMode(SN74HC595_SRCLK_PIN, OUTPUT);
  pinMode(SN74HC95_OE_PIN, OUTPUT);

  setBrightness(255);
}

void loop()
{
  uint16_t distance = sensor.read();
  if (sensor.timeoutOccurred()) {
    Serial.println(" TIMEOUT");
    return;
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" mm");

  int numLEDs = distance / 50;
  if (numLEDs > 8) numLEDs = 8;

  byte LED_PATTERN_SHIFT = (1 << numLEDs) - 1;

  Serial.print("LEDs to light: ");
  Serial.println(numLEDs);
  Serial.print("LED pattern (binary): ");
  Serial.println(LED_PATTERN_SHIFT, BIN);
  Serial.print("LED pattern (decimal): ");
  Serial.println(LED_PATTERN_SHIFT);

  updateShiftRegister(LED_PATTERN_SHIFT);

  delay(100);
}

void updateShiftRegister(byte pattern)
{
  digitalWrite(SN74HC595_RCLK_PIN, LOW);
  shiftOut(SN74HC595_SER_PIN, SN74HC595_SRCLK_PIN, LSBFIRST, pattern);
  digitalWrite(SN74HC595_RCLK_PIN, HIGH);
}

void setBrightness(byte brightness)
{
  analogWrite(SN74HC95_OE_PIN, 255 - brightness);
}
