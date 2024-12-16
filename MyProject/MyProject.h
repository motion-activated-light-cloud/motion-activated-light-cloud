#ifndef MYPROJECT_H
#define MYPROJECT_H
#define MOTION_DURATION 300000

// Enum for states: LIGHT_OFF, LIGHT_ON, MODE_CHANGE, COLOR_CHANGE
typedef enum {
  sLIGHT_OFF = 1,
  sLIGHT_ON = 2,
  sMODE_CHANGE = 3,
  sCOLOR_CHANGE = 4,
} state;

// Function that updates FSM
state updateFSM(state curState, long mils);

#endif



