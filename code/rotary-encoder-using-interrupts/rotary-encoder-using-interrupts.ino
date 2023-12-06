const byte CLK = 9;
const byte DT = 8;

volatile byte state = LOW;
byte prevState = LOW;

void setup() {
  Serial.begin(9600);

  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);

  PCICR = 0B00000001;
  PCIFR = 0B00000001;
  PCMSK0 |= (1 << PCINT5);
}

void loop() {
  if (state != prevState) {
    handleRotaryEncoder();
    prevState = !prevState;
  }
}

ISR(PCINT0_vect) {
  state = !state;
}

void handleRotaryEncoder() {
  if (digitalRead(CLK) == HIGH) {
    if (digitalRead(DT) == LOW) {
      Serial.println("CW");
    } else {
      Serial.println("CCW");
    }
  }
}
