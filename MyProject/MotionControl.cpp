#include "MotionControl.h"
#include "LEDControl.h"
#include "MyWDT.h"
#define PIR1_PIN 5
#define PIR2_PIN 6

bool motionDetected = false;
unsigned long lastMotionTime = 0;

/*
Function that sets the pins of the PIR motion sensors to be inputs and then pets the watchdog.
No params / return
*/
void initializeMotionSensors() {
  pinMode(PIR1_PIN, INPUT);
  pinMode(PIR2_PIN, INPUT);
  petWatchdog();
}

/*
Function that reads PIR sensors to check for motion and then pets watchdog. 
Sets motionDetected to be true when either sensor is HIGH, and sets the lastMotion time to be 
the current time to indicate the time of motion detection. Sets motionDetected to false if 
no motion detection (if both sensors are LOW). Prints to serial monitor based on detection.
No params / return
*/
void checkMotion() {
  bool pir1State = digitalRead(PIR1_PIN);
  bool pir2State = digitalRead(PIR2_PIN);

  if (pir1State == HIGH || pir2State == HIGH) {
    if (pir1State == HIGH) Serial.println("Motion detected by Sensor 1! Starting timer.");
    if (pir2State == HIGH) Serial.println("Motion detected by Sensor 2! Starting timer.");
    motionDetected = true;
    lastMotionTime = millis();
  } else if (pir1State == LOW && pir2State == LOW) {
    Serial.println("No motion detected");
    motionDetected = false;
  }
  petWatchdog();
}

