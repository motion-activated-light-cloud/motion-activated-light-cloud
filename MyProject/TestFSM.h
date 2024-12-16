/* 
Inputs for testing
Note that flexValue is not included in our FSM diagram bc our FSM function doesn't need to know about it,
however it is inlcuded here so we can control how the flex sensor responds to bend.
*/
typedef struct {
  bool flexDetected;
  int flexValue;
  bool motionDetected;
  unsigned long lastMotionTime;
  long mils;
} state_inputs;

/*
Variables for testing
*/
typedef struct {
  unsigned long lastTurnedOff;
  int colorMode;
  uint8_t baseRed;
  uint8_t baseGreen;
  uint8_t baseBlue;
  uint8_t targetRed;
  uint8_t targetGreen;
  uint8_t targetBlue;
  float blendFactor;
  bool buttonPressed;
} state_vars;

// Function for checking that FSM takes the correct transition
bool testTransition(state startState, state endState, state_inputs testStateInputs, state_vars startStateVars, state_vars endStateVars, bool verbos);
// Helper function that lets us print the state
char* s2str(state s);
// Function that runs all unit tests 
bool testAllTests();