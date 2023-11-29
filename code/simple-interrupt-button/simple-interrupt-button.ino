volatile byte state = LOW;

const byte buttonPin = 3;
const byte ledPin = 17;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  // Try doing this without the method attachInterrupt() but by adressing the register of the 32U4
  attachInterrupt(digitalPinToInterrupt(buttonPin), interruptService, FALLING);
}

void loop()
{
    digitalWrite(ledPin, state);
} 

void interruptService()
{
  state = !state;
}