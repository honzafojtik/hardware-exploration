#include <SPI.h>

// define a pin on which the button is
const int BUTTON_PIN = 9;

// volatile long for millis as it changes rapidly
volatile unsigned long TIMER_MILIS = 0;

// variables for calculating the duration of a press and storing previous state of a button
unsigned long pressStartTime = 0;
unsigned long pressDur = 0;
bool prevState = false;

// define what counts as a long press
const unsigned long LONG_PRESS_MS = 50;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  setupTimer1();
}

void loop()
{
  bool currState = digitalRead(BUTTON_PIN) == LOW;

  if (currState && !prevState)
  {
    // button was just pressed
    pressStartTime = TIMER_MILIS;
  }
  
  else if (!currState && prevState)
  {
    // button was just released
    pressDur = TIMER_MILIS - pressStartTime;
      Serial.print(pressDur);
      Serial.print(" ms --> ");

    if (pressDur > LONG_PRESS_MS)
      {
        Serial.print("Long press!");
      }

    else
      {
        Serial.print("Short press!");
      }
      Serial.println();
  }
  
  prevState = currState;

}

void setupTimer1()
{
  // clear Control Registers A and B
  TCCR1A = 0;
  TCCR1B = 0;

  // set to CTC (Clear Timer on Compare Match) mode
  TCCR1B |= (1 << WGM12);

  // set prescaler to 64, for 1 tick = 1 us at 16 MHz clock
  TCCR1B |= (1 << CS11) | (1 << CS10);

  // set the compare match value for 1ms increment
  OCR1A = 2499; // ( 16MHz / 64 ) / 1000 - 1

  // enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  // enable global interrupts
  sei(); 
}

ISR(TIMER1_COMPA_vect)
{
  TIMER_MILIS++;
}


