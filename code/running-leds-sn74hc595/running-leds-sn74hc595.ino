const int SN74HC595_SER_PIN = A0;
const int SN74HC595_RCLK_PIN = A1;
const int SN74HC595_SRCLK_PIN = A2;
const int SN74HC95_OE_PIN = 3;

byte LED_PATTERN = 0;
byte LED_PATTERN_SHIFT = 0;

void setup()
{
  // SN74HC595 pin setup
  pinMode(SN74HC595_SER_PIN, OUTPUT);
  pinMode(SN74HC595_RCLK_PIN, OUTPUT);
  pinMode(SN74HC595_SRCLK_PIN, OUTPUT);
  pinMode(SN74HC95_OE_PIN, OUTPUT);
}

void loop()
{
  setBrightness(255); // 0 - 255
  for (int i = 0; i < 8; i++)
  {
    Serial.print("BEFORE SHIFT:");
    Serial.println(LED_PATTERN_SHIFT);
    LED_PATTERN_SHIFT = LED_PATTERN << i;
    Serial.print("AFTER SHIFT:");
    Serial.println(LED_PATTERN_SHIFT);
    updateShiftRegister(LED_PATTERN_SHIFT);
    delay(100);
  }

}

void updateShiftRegister(byte pattern)
{
  digitalWrite(SN74HC595_RCLK_PIN, LOW);
  shiftOut(SN74HC595_SER_PIN, SN74HC595_SRCLK_PIN, LSBFIRST, pattern);
  digitalWrite(SN74HC595_RCLK_PIN, HIGH);
}

void setBrightness(byte brightness)
{
  analogWrite(SN74HC95_OE_PIN, 255-brightness);
}