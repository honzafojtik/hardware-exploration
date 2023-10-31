#include <SPI.h>
#include <math.h>

const int MCP3008_CS_PIN = 10;

const int SN74HC595_SER_PIN = 16;
const int SN74HC595_RCLK_CS_PIN = 9;
const int SN74HC595_SRCLK_PIN = 15;
const int SN74HC95_OE_PIN = 3;

const byte START_BYTE = 1;
const byte DIFFMODE = 128; 

byte LED_PATTERN = 1;

SPISettings mySettings(2000000, MSBFIRST, SPI_MODE0);

void setup()
{
  // begin SPI
  SPI.begin();

  // SN74HC595 pin setup
  pinMode(SN74HC595_SER_PIN, OUTPUT);
  pinMode(SN74HC595_RCLK_CS_PIN, OUTPUT);
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

  int bitPotValue = map(readPot(bitPotChannel), 0, 1000, 0, 255);
  int brightPotValue = map(readPot(brightPotChannel), 0, 1023, 0, 255);

  Serial.print("BIT MAPPED: ");
  Serial.println(bitPotValue);

  Serial.print("BRI MAPPED: ");
  Serial.println(brightPotValue);

  Serial.println();

  setBrightness(brightPotValue);
  // LED_PATTERN = pow(2, bitPotValue);
  // LED_PATTERN = bitPotValue;
  for (int i = 0; i < 255; i++)
  {
    updateShiftRegister(i);
    delay(50);
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
  SPI.beginTransaction(mySettings);

  digitalWrite(SN74HC595_RCLK_CS_PIN, LOW);

  SPI.transfer(pattern);

  digitalWrite(SN74HC595_RCLK_CS_PIN, HIGH);

  SPI.endTransaction();
}