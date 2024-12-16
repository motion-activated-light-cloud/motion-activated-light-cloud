#include "LEDControl.h"
#include "MyWDT.h"
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define FLEX_SENSOR_PIN A0
#define BUTTON_PIN 8
int flexValue = 0;
bool flexDetected = false;
bool buttonPressed = false;
int colorMode = 0;
unsigned long lastTurnedOff = 0;


// Color blending variables
uint8_t baseRed = 200, baseGreen = 60, baseBlue = 30;
uint8_t targetRed = 170, targetGreen = 0, targetBlue = 170;
float blendFactor = 0;

uint32_t colors[] = {
  strip.Color(200, 60, 30),    // Pink
  strip.Color(30, 255, 60),    // Mint Green
  strip.Color(30, 60, 150),    // Blue
  strip.Color(255, 100, 0),  // Orange
  strip.Color(0, 255, 255),  // Cyan
  strip.Color(255, 0, 255),  // Magenta
  strip.Color(255, 255, 255) // White
};

uint32_t targetColors[] = {
  strip.Color(170, 0, 170),  // Pink -> Purple
  strip.Color(255, 255, 0),  // Mint Green -> Yellow
  strip.Color(30, 255, 60),  // Blue -> Mint Green
  strip.Color(247, 215, 5),  // Orange-> Yellow
  strip.Color(0, 0, 255),    // Cyan -> Blue
  strip.Color(255, 0, 0),    // Magenta -> Red
  strip.Color(0, 0, 0)       // White -> Off
};

/*
Function that sets up the LEDs and then pets the watchdog, called in setup.
No params / return
*/
void initializeLEDs() {
  strip.begin();
  strip.show();
  petWatchdog();
}

/*
Function that turns on the LEDs, showing current base color. 
One of the output functions for FSM.
No params / return
*/
void turnOnLEDs() {
  strip.fill(strip.Color(baseRed, baseGreen, baseBlue));
  strip.show();
}

/*
Function that turns off the LEDs.
One of the output functions for FSM/
No params / return
*/
void turnOffLEDs() {
  strip.fill(0, 0, 0);
  strip.show();
}

/*
Function that reads the flex sensor, sets the value to be flexValue, and then pets the watchdog.
If the new value is greater than the last one by 10, then flexDetected is set to true.
No params / return
*/
void checkFlexSensor() {
  int lastFlexValue = flexValue;
  flexValue = analogRead(FLEX_SENSOR_PIN);
  if (abs(flexValue - lastFlexValue) >= 10) {
    flexDetected = true;
  } else {
    flexDetected = false;
  }
  petWatchdog();
}

/*
Function that uses the current flexValue to determine how to change the LED color.
Uses map function to convert the flexValue to a decimal (corresponding to the degree of bend),
then uses that decimal to blend from the base color to the target color. Then, it 
changes the LEDs based on the newly calculatuted color.
One of the output functions for FSM
No params / return
*/
void handleFlexSensor() {
  blendFactor = constrain(map(flexValue, 50, 950, 0, 100) / 100.0, 0.0, 1.0);
  uint8_t blendedRed =baseRed + blendFactor * (targetRed - baseRed);
  uint8_t blendedGreen =baseGreen + blendFactor * (targetGreen - baseGreen);
  uint8_t blendedBlue =baseBlue + blendFactor * (targetBlue - baseBlue);

  strip.fill(strip.Color(blendedRed, blendedGreen, blendedBlue));
  strip.show();

  // Serial.print("Flex Sensor: ");
  // Serial.print(flexValue);
  // Serial.print(" -> Blended Color: R=");
  // Serial.print(blendedRed);
  // Serial.print(", G=");
  // Serial.print(blendedGreen);
  // Serial.print(", B=");
  // Serial.println(blendedBlue);
}

/*
Function that increments the colorMode within 0-8, to determine the current
set of base and target colors to work with. Turns on LEDs to new color, sets buttonPressed to true, 
and pets watchdog. 
No params / return
*/
void checkButtonPress() {

  colorMode = (colorMode + 1) % (sizeof(colors) / sizeof(colors[0]));

  uint32_t baseColor = colors[colorMode];
  uint32_t targetColor = targetColors[colorMode];

  baseRed = (baseColor >> 16) & 0xFF;
  baseGreen = (baseColor >> 8) & 0xFF;
  baseBlue = baseColor & 0xFF;

  targetRed = (targetColor >> 16) & 0xFF;
  targetGreen = (targetColor >> 8) & 0xFF;
  targetBlue = targetColor & 0xFF;
  buttonPressed = true;
  strip.fill(strip.Color(baseRed, baseGreen, baseBlue));
  strip.show();

  petWatchdog();
  
}
