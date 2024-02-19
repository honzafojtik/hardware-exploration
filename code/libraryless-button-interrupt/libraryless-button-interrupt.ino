const byte buttonPin = 9;
// const byte ledPin = 8;

volatile byte state = LOW;

void setup()
{
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  // pinMode(ledPin, OUTPUT);

  // Setting the I-bit in the Status Register (SREG) to one
  sei();
  
  // Setting the PCIE0 bit of the PCICR (Pin Change Interrupt Control Register) to one
  PCICR = 0B00000001;
  // Setting the PCIF0 of the PCIFR (Pin Change Interrupt Flat Register) to one, by that clearing it, see page 91 11.1.6 of the datasheet
  PCIFR = 0B00000001;
  // Setting the Pin Change Mask Register to expect an interrupt on hardware pin 9, i.e. PCINT5
  PCMSK0 |= (1 << PCINT5);
}

void loop()
{
  // digitalWrite(ledPin, state);
  Serial.println(state);
}

ISR(PCINT0_vect)
{
  // Check register/read a port to see if a button is HIGH or LOW thus if the edge is falling or rising
  state = !state;
}
