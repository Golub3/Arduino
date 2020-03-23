#include "TimerThree.h"

const int led1 = 2; 
const int led2 = 3;
const int led3 = 4;

const int pot = A0;
const double pi = 3.14;

double k;
double t;


void setup() {
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(pot, INPUT);
  
  Timer3.initialize(16000);
  Timer3.attachInterrupt(func);  
  
}


void func() {
  
  t = millis()/double(1000);
  k = analogRead(pot)/int(100);
  
  analogWrite(led1, 255*(sin(k*t)+1)/2);
  analogWrite(led2, 255*(sin(k*t+2*pi/3)+1)/2);
  analogWrite(led3, 255*(sin(k*t+4*pi/3)+1)/2);
  
}


void loop() {    
}
