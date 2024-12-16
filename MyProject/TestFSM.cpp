#include "MyProject.h"
#include "LEDControl.h"
#include "MotionControl.h"
#include "MyWDT.h"
#include "TestFSM.h"

// Colors for reference
// uint32_t colors[] = {
//   strip.Color(200, 60, 30),    // Pink
//   strip.Color(30, 255, 60),    // Mint Green
//   strip.Color(30, 60, 150),    // Blue
//   strip.Color(255, 100, 0),  // Orange
//   strip.Color(0, 255, 255),  // Cyan
//   strip.Color(255, 0, 255),  // Magenta
//   strip.Color(255, 255, 255) // White
// };

// uint32_t targetColors[] = {
//   strip.Color(170, 0, 170),  // Pink -> Purple
//   strip.Color(255, 255, 0),  // Mint Green -> Yellow
//   strip.Color(30, 255, 60),  // Blue -> Mint Green
//   strip.Color(247, 215, 5),  // Orange-> Yellow
//   strip.Color(0, 0, 255),    // Cyan -> Blue
//   strip.Color(255, 0, 0),    // Magenta -> Red
//   strip.Color(0, 0, 0)       // White -> Off
// };

/*
Function for checking that FSM takes the correct transition.
params:
- startState: state before the transition
- endState: state after the transition
- testStateInputs: inputs to the FSM (see TestFSM.h for full list of inputs and vars)
- startStateVars: variables of the FSM before the transition 
- endStateVars: variables of the FSM after the transition
- verbos: boolean determining what gets printed to Serial monitor
returns true if all variables match the expected variables after a transition is taken
prints comparison of variables otherwise
*/
bool testTransition(state startState, state endState, state_inputs testStateInputs, state_vars startStateVars, state_vars endStateVars, bool verbos){
  lastTurnedOff = startStateVars.lastTurnedOff;
  colorMode = startStateVars.colorMode;
  baseRed = startStateVars.baseRed;
  baseGreen = startStateVars.baseGreen;
  baseBlue = startStateVars.baseBlue;
  targetRed = startStateVars.targetRed;
  targetGreen = startStateVars.targetGreen;
  targetBlue = startStateVars.targetBlue;
  blendFactor = startStateVars.blendFactor;
  buttonPressed = startStateVars.buttonPressed;

  flexDetected = testStateInputs.flexDetected;
  flexValue = testStateInputs.flexValue;
  motionDetected = testStateInputs.motionDetected;
  lastMotionTime = testStateInputs.lastMotionTime;

  if (startStateVars.buttonPressed == true) checkButtonPress(); 
  state resultState = updateFSM(startState, testStateInputs.mils);

  bool passedTest = (endState == resultState and lastTurnedOff == endStateVars.lastTurnedOff and
                    colorMode == endStateVars.colorMode and 
                    baseRed == endStateVars.baseRed and
                    baseGreen == endStateVars.baseGreen and
                    baseBlue == endStateVars.baseBlue and
                    targetRed == endStateVars.targetRed and
                    targetGreen == endStateVars.targetGreen and
                    targetBlue == endStateVars.targetBlue and
                    blendFactor == endStateVars.blendFactor);

  if (! verbos) {
    return passedTest;
  } else if (passedTest) {
    char sToPrint[200];
    sprintf(sToPrint, "Test from %s to %s PASSED", s2str(startState), s2str(endState));
    Serial.println(sToPrint);
    return true;
  } else {
    char sToPrint[200];
    Serial.println(s2str(startState));
    sprintf(sToPrint, "Test from %s to %s FAILED", s2str(startState), s2str(endState));
    Serial.println(sToPrint);
    sprintf(sToPrint, "End state expected: %s | actual: %s", s2str(endState), s2str(resultState));
    Serial.println(sToPrint);
    sprintf(sToPrint, "Inputs: flexDetected %d | flexValue %d | motionDetected %d | lastMostionTime %lu | mils %ld", testStateInputs.flexDetected, testStateInputs.flexValue, testStateInputs.motionDetected, testStateInputs.lastMotionTime, testStateInputs.mils);
    Serial.println(sToPrint);
    sprintf(sToPrint, "          %13s | %9s | %7s | %9s | %8s | %9s | %11s | %10s | %11s | %13s", "lastTurnedOff", "colorMode", "baseRed", "baseGreen", "baseBlue", "targetRed", "targetGreen", "targetBlue", "blendFactor", "buttonPressed");
    Serial.println(sToPrint);
    sprintf(sToPrint, "expected: %lu | %d | %3u | %3u | %3u | %3u | %3u | %3u | %f | %d", endStateVars.lastTurnedOff, endStateVars.colorMode, endStateVars.baseRed, endStateVars.baseGreen, endStateVars.baseBlue, endStateVars.targetRed, endStateVars.targetGreen, endStateVars.targetBlue, endStateVars.blendFactor, endStateVars.buttonPressed);
    Serial.println(sToPrint);
    sprintf(sToPrint, "actual:   %lu | %d | %3u | %3u | %3u | %3u | %3u | %3u | %f | %d", lastTurnedOff, colorMode, baseRed, baseGreen, baseBlue, targetRed, targetGreen, targetBlue, blendFactor, buttonPressed);
    Serial.println(sToPrint);
    return false;
  }
}

/*
Helper function that takes in a state and returns the corresponding char pointer.
*/
char* s2str(state s) {
  switch(s) {
    case sLIGHT_OFF:
    return "(1) LIGHT_OFF";
    case sLIGHT_ON:
    return "(2) LIGHT_ON";
    case sMODE_CHANGE:
    return "(3) MODE_CHANGE";
    case sCOLOR_CHANGE:
    return "(4) COLOR_CHANGE";
    default:
    return "??";
  }
}


/*
15 test cases for unit testing. See spreadsheet in test plan for more detail.
*/
const int numTests = 15;

// Start states
const state testStatesIn[numTests] = {(state) 1, (state) 1, (state) 1, (state) 2, (state) 2, (state) 2, (state) 2, (state) 3, (state) 3, (state) 3, (state) 3, (state) 4, (state) 4, (state) 4, (state) 4};

// End states
const state testStatesOut[numTests] = {(state) 1, (state) 1, (state) 2, (state) 1, (state) 2, (state) 3, (state) 4, (state) 1, (state) 2, (state) 3, (state) 4, (state) 1, (state) 2, (state) 3, (state) 4};

// Inputs for testing
const state_inputs testInputs[numTests] = {
  // flexDetected | flexValue | motionDetected | lastMotionTime | mils
  {false, 0, false, 0, 1000}, // 1-1
  {false, 0, true, 0, 1000}, // 1-1
  {false, 0, true, 0, 15000}, // 1-2
  {false, 0, false, 5000, MOTION_DURATION + 5000}, // 2-1
  {false, 0, false, 5000, MOTION_DURATION - 5000}, // 2-2
  {false, 0, false, 5000, MOTION_DURATION - 5000}, // 2-3
  {true, 700, false, 5000, MOTION_DURATION - 5000}, // 2-4
  {false, 0, false, 5000, MOTION_DURATION + 5000}, // 3-1
  {false, 0, true, 6000, MOTION_DURATION - 6000}, // 3-2
  {false, 0, true, 6000, MOTION_DURATION - 6000}, // 3-3
  {true, 300, true, 6000, MOTION_DURATION - 6000}, // 3-4
  {false, 0, false, 5000, MOTION_DURATION + 10000}, // 4-1
  {false, 0, false, 5000, MOTION_DURATION - 10000}, // 4-2
  {false, 0, false, 5000, MOTION_DURATION - 10000}, // 4-3
  {true, 500, false, 5000, MOTION_DURATION - 10000} // 4-4
  };

// Variables before transition
const state_vars testVarsIn[numTests] = {
  // lastTurnedOff | colorMode | baseRed | baseGreen | baseBlue | targetRed | targetGreen | targetBlue | blendFactor | buttonPressed
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 1-1
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 1-1
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 1-2
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 2-1
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 2-2
  {0, 0, 200, 60, 30, 170, 0, 170, 0, true}, // 2-3
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 2-4 
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false},  // 3-1
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false},  // 3-2
  {0, 1, 30, 255, 60, 255, 255, 0, 0, true},  // 3-3
  {0, 1, 30, 255, 60, 255, 255, 0, 0.8, false},  // 3-4
  {0, 1, 30, 255, 60, 255, 255, 0, 0.8, false},  // 4-1
  {0, 1, 30, 255, 60, 255, 255, 0, 0.8, false}, // 4-2
  {0, 1, 30, 255, 60, 255, 255, 0, 0, true}, // 4-3
  {0, 2, 30, 60, 150, 30, 255, 60, 0, false} // 4-4
  };

// Variables after transition
const state_vars testVarsOut[numTests] = {
  // lastTurnedOff | colorMode | baseRed | baseGreen | baseBlue | targetRed | targetGreen | targetBlue | blendFactor | buttonPressed
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 1-1
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 1-1
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 1-2
  {MOTION_DURATION + 5000, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 2-1
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false}, // 2-2
  {0, 1, 30, 255, 60, 255, 255, 0, 0, false}, // 2-3
  {0, 0, 200, 60, 30, 170, 0, 170, constrain(map(700, 50, 950, 0, 100) / 100.0, 0.0, 1.0), false}, // 2-4 
  {MOTION_DURATION + 5000, 0, 200, 60, 30, 170, 0, 170, 0, false},  // 3-1
  {0, 0, 200, 60, 30, 170, 0, 170, 0, false},  // 3-2
  {0, 2, 30, 60, 150, 30, 255, 60, 0, false},  // 3-3
  {0, 1, 30, 255, 60, 255, 255, 0, constrain(map(300, 50, 950, 0, 100) / 100.0, 0.0, 1.0), false},  // 3-4
  {MOTION_DURATION + 10000, 1, 30, 255, 60, 255, 255, 0, 0.8, false},  // 4-1
  {0, 1, 30, 255, 60, 255, 255, 0, 0.8, false}, // 4-2
  {0, 2, 30, 60, 150, 30, 255, 60, 0, false}, // 4-3
  {0, 2, 30, 60, 150, 30, 255, 60, constrain(map(500, 50, 950, 0, 100) / 100.0, 0.0, 1.0), false} // 4-4
  };


/*
Runs through all the test cases defined above. Prints confirmation of passed test or 
list of variables / inputs / states if any test does not pass. 
Returns true if all tests passed, false otherwise.
 */
bool testAllTests() {
  for (int i = 0; i < numTests; i++) {
    Serial.print("Running test ");
    Serial.println(i);
    if (!testTransition(testStatesIn[i], testStatesOut[i], testInputs[i], testVarsIn[i], testVarsOut[i], true)) {
      return false;
    }
    Serial.println();
  }
  Serial.println("All tests passed!");
  return true;
}

