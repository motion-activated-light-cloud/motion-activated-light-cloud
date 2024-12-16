#include "MyWDT.h"

unsigned long wdtMil = 0;

// Function that sets up watchdog based on interval (2684). No param / return
void setupWDT() {
  if(wdtInterval < 1) {
    // Serial.println("Invalid watchdog interval");
    while(1){}
  }
  if (WDT.begin(wdtInterval)) {
    // Serial.print("WDT interval: ");
    WDT.refresh();
    // Serial.print(WDT.getTimeout());
    WDT.refresh();
    // Serial.println(" ms");
    WDT.refresh();
  } else {
    // Serial.println("Error initializing watchdog");
    while(1){}
  }
}

// Function that pets the watchdog. No param / return
void petWatchdog() {
  WDT.refresh(); 
  wdtMil = millis();
}