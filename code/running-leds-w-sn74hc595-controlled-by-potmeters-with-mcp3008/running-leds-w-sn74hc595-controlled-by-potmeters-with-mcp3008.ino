#include <SPI.h>
#include <math.h>

const int MCP3008_CS_PIN = 10;

con
const int SN74HC595_SER_PIN = A0;
const int SN74HC595_RCLK_PIN = A1;
const int SN74HC595_SRCLK_PIN = A2;
const int SN74HC95_OE_PIN = 3;

const byte START_BYTE = 1;
const byte DIFFMODE = 128; 

byte LED_PATTERN = 0;
byte LED_PATTERN_SHIFT = 0;

SPISettings mySettings(2000000, MSBFIRST, SPI_MODE0);

void setup()
{
  // begin SPI
  SPI.begin();

  // SN74HC595 pin setup
  pinMode(SN74HC595_SER_PIN, OUTPUT);
  pinMode(SN74HC595_RCLK_PIN, OUTPUT);
  pinMode(SN74HC595_SRCLK_PIN, OUTPUT);
  pinMode(SN74HC95_OE_PIN, OUTPUT);

  // MCP3008 pin setup
  pinMode(MCP3008_CS_PIN, OUTPUT);
  digitalWrite(MCP3008_CS_PIN, LOW);
  digitalWrite(MCP3008_CS_PIN, HIGH);
}

void loop()
{
  byte bitPotChannel = 0;
  byte brightPotChannel = 2;

  int bitPotValue = map(readPot(bitPotChannel), 0, 950, 0, 7);
  int brightPotValue = map(readPot(brightPotChannel), 0, 1023, 0, 255);

  Serial.print("BIT MAPPED: ");
  Serial.println(bitPotValue);

  Serial.print("BRI MAPPED: ");
  Serial.println(brightPotValue);

  Serial.println();

  setBrightness(brightPotValue);
  LED_PATTERN = pow(2, bitPotValue);
  // LED_PATTERN = bitPotValue;
  for (int i = 0; i < 8; i++)
  {
    LED_PATTERN_SHIFT = LED_PATTERN << i;
    updateShiftRegister(LED_PATTERN_SHIFT);
    delay(100);
  }
}

int readPot(byte channel)
{
  // begin SPI transaction using the decalred SPISettings variable
  SPI.beginTransaction(mySettings);

  // set chip select pin to LOW to be able to transfer data, see page 21 of datasheet
  digitalWrite(MCP3008_CS_PIN, LOW);

  // transfer the START_BYTE, i.e. 0000 0001, see page 21 of datasheet
  SPI.transfer(START_BYTE);

  // set the differential mode to single-ended input configuration by sending the 1000 0000 byte, see page 19 table 5-2 of the datasheet
  // add channel which is a byte, in this case 0000 0000, thus it does nothing and perform shift left on it
  // the bitwise AND with 0x03 is performed on the MSB (the return of the SPI.transfer function )
  byte MSB = SPI.transfer(DIFFMODE + channel << 4) & 0x03;
  byte LSB = SPI.transfer(0);

  digitalWrite(MCP3008_CS_PIN, HIGH);

  SPI.endTransaction();

  return MSB << 8 | LSB;
}

void setBrightness(byte brightness)
{
  analogWrite(SN74HC95_OE_PIN, 255-brightness);
}

void updateShiftRegister(byte pattern)
{
  digitalWrite(SN74HC595_RCLK_PIN, LOW);
  shiftOut(SN74HC595_SER_PIN, SN74HC595_SRCLK_PIN, LSBFIRST, pattern);
  digitalWrite(SN74HC595_RCLK_PIN, HIGH);
}