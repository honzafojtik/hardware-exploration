const byte CLK = 9;
const byte DT = 8;
const byte SER = 10; // Servo pin

volatile byte state = LOW;
volatile byte prevState = LOW;

int counter = 0;

// Duration of rotation per step
const int rotationDuration = 80; // Adjust this value based on experimentation

void setup() {
  Serial.begin(9600);

  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SER, OUTPUT);

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
      counter--;
      rotateServo(rotationDuration); // Rotate CCW
      Serial.print("CCW | ");
      Serial.println(counter);
    } else {
      counter++;
      rotateServo(-rotationDuration); // Rotate CW
      Serial.print("CW  | ");
      Serial.println(counter);
    }
  }
}

void rotateServo(int direction) {
  int pulseWidth = (direction < 0) ? 1000 : 2000; // 1000 for CCW, 2000 for CW
  int rotationTime = 50; // Calibrated time for 18 degrees rotation, adjust based on experimentation

  // Rotate the servo
  digitalWrite(SER, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(SER, LOW);
  delay(rotationTime); // Delay for the duration of rotation

  // Stop the servo
  digitalWrite(SER, HIGH);
  delay(15); // Pulse width to stop the servo
  digitalWrite(SER, LOW);
  delay(20); // Complete the cycle
}
