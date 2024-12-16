#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Adafruit_NeoPixel.h>

#define LED_PIN 3
#define NUM_LEDS 144

extern int flexValue; // Keeps track of value provided by flex sensor
extern bool flexDetected; // Boolean indicating if flex was detected or not
extern bool buttonPressed; // Boolean indicating if button was pressed or not
extern int colorMode; // Int keeping track of current color mode (can be 0 - 6)
extern unsigned long lastTurnedOff; // Time at which the light was last turned off
extern float blendFactor; // Decimal indicating how much to blend base color to target color (used for gradient effect)

// Base color that shows when flex sensor is not bent 
extern uint8_t baseRed; 
extern uint8_t baseGreen;
extern uint8_t baseBlue; 
// Target color to approach as the flex sensor is more bent
extern uint8_t targetRed; 
extern uint8_t targetGreen; 
extern uint8_t targetBlue; 

void initializeLEDs(); // Function that sets up the LEDs
void turnOnLEDs(); // Function that turns on LEDs
void turnOffLEDs(); // Function that turns off LEDs
void checkFlexSensor(); // Function that reads flex sensor input
void handleFlexSensor(); // Function that uses flex sensor input to get corresponding color based on bend
void checkButtonPress(); // Function that changes colorMode, base and target colors, and buttonPressed based on an interrupt

#endif
