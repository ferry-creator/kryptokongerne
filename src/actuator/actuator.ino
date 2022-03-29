#include <Arduino.h>
#include <analogWrite.h>

// Motor control digital output pins defined as global constants
#define controlPin1A 2
#define controlPin2A 5
#define enablePin 9

boolean motorDirection = 1;   // 1 -> forward, 0 -> backwards
int motorSpeed = 0;           // Motor speed 0 <> 255

int strength = 0;             // get from app
#define strengthMax 5
#define speedMin 72
#define speedMax 255

int speedFromStrength(int x) {
  // equation, x in range [a,b]:
  // x(a,b) = (b-a) * ( (x - x[min]) / (x[max] - x[min]) ) + a
  // x[min] assumed 0:
  return (speedMax - speedMin) * ( x / strengthMax ) + speedMin
}

void SetMotorControl()
{
  if (motorDirection == 1)          // forward
  {
    digitalWrite(controlPin1A, HIGH);
    digitalWrite(controlPin2A, LOW);
  }
  else                              // reverse
  {
    digitalWrite(controlPin1A, LOW);
    digitalWrite(controlPin2A, HIGH);
  }
  analogWrite(enablePin, motorSpeed);
}

void setup() 
{
  Serial.begin(9600);
  
  // Declare digital output pins:
  pinMode(controlPin1A, OUTPUT);    // 1A
  pinMode(controlPin2A, OUTPUT);    // 2A
  pinMode(enablePin, OUTPUT);       // EN1,2
 
  digitalWrite(enablePin, LOW);     // motor off
}

void loop() {
  motorDirection = 1;
  motorSpeed = speedFromStrength(strength);
  SetMotorControl();
  
  delay(100);
}
