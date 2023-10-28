#include <SPI.h>

// pin definitions
const int MCP3008_CS_PIN = 10;

const byte START_BYTE = 1; // see datahsheet page 21 about start bit
const byte DIFFMODE = 128; // set differential mode byte sending 1000 0000 byte

// declaring the SPISettings variable, using 2MHz, Most Significant Bit First and SPI_MODE0
SPISettings mySettings(2000000, MSBFIRST, SPI_MODE0);

void setup() {

  // begin SPI
  SPI.begin();

  // MCP3008 pin setup
  // set chip select pin to output and first write it LOW and then HIGH to make sure the pin is not floating
  pinMode(MCP3008_CS_PIN, OUTPUT);
  digitalWrite(MCP3008_CS_PIN, LOW);
  digitalWrite(MCP3008_CS_PIN, HIGH);
}

void loop() {

  // set potentiometer channel based on which channel on the MCP3008 is being used
  byte bitPotChannel = 0;
  byte brightPotChannel = 1;

  // read the potentiometer value based on the channel it is connected to
  int bitPotValue = readPot(bitPotChannel);
  int brightPotValue = readPot(brightPotChannel);

  // map the values
  Serial.print("Bit potmeter value: ");
  Serial.println(map(bitPotValue, 0, 950, 0, 7));
  // Serial.println(bitPotValue);
  Serial.print("Bright potmeter value: ");
  Serial.println(map(brightPotValue, 0, 950, 0, 255));
  // Serial.println(brightPotValue);
  Serial.println();
  // 100 ms delay between reads
  delay(100);
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