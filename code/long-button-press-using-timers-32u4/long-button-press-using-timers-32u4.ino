const byte buttonPin = 9;
volatile byte buttonState = LOW;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  // Setup pin change interrupt
  PCICR = 0B00000001;
  PCIFR = 0B00000001;
  PCMSK0 |= (1 << PCINT5);

  Serial.println("[INFO] Setup complete");
}

void loop()
{
  Serial.println(buttonState);
}

ISR(PCINT0_vect)
{
  buttonState = !buttonState;
}