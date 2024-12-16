#include "MyProject.h"
#include "MotionControl.h"
#include "LEDControl.h"
#include "MyWDT.h"
#include "TestFSM.h"

/*
Sets up watchdog, motion sensors, LEDs, interrupt. Has function to run unit tests.
No params / return
*/
void setup() {
  Serial.begin(9600);
  while(!Serial);
  setupWDT();
  initializeMotionSensors();
  initializeLEDs();
  Serial.println("System set up complete.");
  attachInterrupt(digitalPinToInterrupt(8), checkButtonPress, RISING);
  // UNCOMMENT THE CODE BELOW TO RUN TESTS -- MAKE SURE THE CODE INSIDE THE LOOP IS COMMENTED OUT
  // testAllTests();
}

/*
Polls for inputs then updates time-triggered FSM.
No params / return.
*/
void loop() {
  static state CURRENT_STATE = sLIGHT_OFF;
  updateInputs();
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis());
  delay(200);
}

/*
Function that updates FSM. Takes current state, and current time as parameters.
Returns next state based on the transition taken. 
*/
state updateFSM(state curState, long mils) {
  state nextState;
  switch(curState) {
    case sLIGHT_OFF:
      if (!motionDetected || mils - lastTurnedOff < 5000) { // transition 1-1
        Serial.println("1-1 transition");
        turnOffLEDs();
        nextState = sLIGHT_OFF;
      }
      else if (motionDetected) { // transition 1-2
        Serial.println("1-2 transition");
        turnOnLEDs();
        nextState = sLIGHT_ON;
      }
      break;
    case sLIGHT_ON:
      if (!motionDetected && (mils - lastMotionTime >= MOTION_DURATION)) { // transition 2-1
        Serial.println("2-1 transition");
        turnOffLEDs();
        lastTurnedOff = mils;
        nextState = sLIGHT_OFF;
      } else if(!buttonPressed && !flexDetected && (mils - lastMotionTime < MOTION_DURATION)){ // transition 2-2
        Serial.println("2-2 transition");
        nextState = sLIGHT_ON;
      } else if (buttonPressed && mils - lastMotionTime < MOTION_DURATION) { // transition 2-3
        Serial.println("2-3 transition");
        buttonPressed = false;
        nextState = sMODE_CHANGE;
      } else if (flexDetected && !buttonPressed && (mils - lastMotionTime < MOTION_DURATION)){ // transition 2-4
        Serial.println("2-4 transition");
        handleFlexSensor();
        nextState = sCOLOR_CHANGE;
      }
      break;
    case sMODE_CHANGE:
      if (!motionDetected && (mils - lastMotionTime >= MOTION_DURATION)) { // transition 3-1
        Serial.println("3-1 transition");
        turnOffLEDs();
        lastTurnedOff = mils;
        nextState = sLIGHT_OFF;
      } else if (!buttonPressed && !flexDetected && (mils - lastMotionTime < MOTION_DURATION)){ // transition 3-2
        Serial.println("3-2 transition");
        nextState = sLIGHT_ON;
      } else if(buttonPressed && mils - lastMotionTime < MOTION_DURATION){ // transition 3-3
        Serial.println("3-3 transition");
        buttonPressed = false;
        nextState = sMODE_CHANGE;
      } else if (flexDetected && !buttonPressed && (mils - lastMotionTime < MOTION_DURATION)){ // transition 3-4
        Serial.println("3-4 transition");
        handleFlexSensor();
        nextState = sCOLOR_CHANGE;
      }
      break;
    case sCOLOR_CHANGE: 
      if (!motionDetected && (mils - lastMotionTime >= MOTION_DURATION)) { // transition 4-1
        Serial.println("4-1 transition");
        turnOffLEDs();
        lastTurnedOff = mils;
        nextState = sLIGHT_OFF;
      } else if (!buttonPressed && !flexDetected && (mils - lastMotionTime < MOTION_DURATION)){ // transition 4-2
        Serial.println("4-2 transition");
        nextState = sLIGHT_ON;
      } else if(buttonPressed && mils - lastMotionTime < MOTION_DURATION){ // transition 4-3
        Serial.println("4-3 transition");
        buttonPressed = false;
        nextState = sMODE_CHANGE;
      } else if (flexDetected && !buttonPressed && (mils - lastMotionTime < MOTION_DURATION)){ // transition 4-4
        Serial.println("4-4 transition");
        handleFlexSensor();
        nextState = sCOLOR_CHANGE;
      }
      break;
  }
  petWatchdog();
  return nextState;
}

/*
Function that polls for inputs (motion sensor input and flex sensor input).
No params / return
*/
void updateInputs() {
  // Check PIR sensor
  checkMotion();
  // Check flex sensor
  checkFlexSensor();
}

