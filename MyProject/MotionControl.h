#ifndef MOTIONCONTROL_H
#define MOTIONCONTROL_H

#include <Arduino.h>

// Boolean indicating if motion was detected or not
extern bool motionDetected;
// Time at which motion was last detected
extern unsigned long lastMotionTime;

// Function that sets up motion sensors
void initializeMotionSensors();
// Function that reads sensors to check for motion 
void checkMotion();

#endif

