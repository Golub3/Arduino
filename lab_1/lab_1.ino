#include <Arduino.h>

const int buttonPin = 2;
const int ledPin =  13;
const int buttonDelay = 150;
unsigned int state;
unsigned long last_time;

void led_int()
{
  unsigned long time = millis();
  if(time - last_time <= buttonDelay) {
    return;
  }
  last_time = time;
  if(state == LOW) {
    digitalWrite(ledPin, LOW);
    state = HIGH;
  } else {
    digitalWrite(ledPin, HIGH);
    state = LOW;
  }
}

void setup()
{
  state = LOW;
  last_time = millis();
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), led_int, RISING);
}

void loop()
{
}
