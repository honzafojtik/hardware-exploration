const byte buttonPin = 9;
const byte ledPin = 8;

volatile byte state = LOW;
volatile byte previousButtonState = HIGH;

void setup()
{
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  PCICR = 0B00000001;
  PCIFR = 0B00000001;
  PCMSK0 |= (1 << PCINT5);
}

void loop()
{
  digitalWrite(ledPin, state);
  Serial.println(state);
}

ISR(PCINT0_vect)
{
  // Check register/read a port to see if a button is HIGH or LOW thus if the edge is falling or rising
  state = !state;
}
