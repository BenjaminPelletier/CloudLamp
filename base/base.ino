// Install WAV Trigger library by cloning https://github.com/robertsonics/WAV-Trigger-Arduino-Serial-Library into Arduino libraries folder (usually Documents/Arduino/libraries)
// Install AltSoftSerial using Tools -> Manage Libraries...

#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include <wavTrigger.h>
#include "buttons.h"

const int PIN_POT = A5;
const int PIN_ONE = A1;
const int PIN_TWO = A0;
const int PIN_THREE = A3;
const int PIN_FOUR = A2;
const int PIN_WATER_LEVEL = A4;
const int PIN_PUMP = 13;

const int PIN_TWISTED_GROUND_RX = 2;
const int PIN_TWISTED_GROUND_TX = 3;

const int BUTTON_COUNT = 4;
const int PIN_BUTTONS[] = {PIN_ONE, PIN_TWO, PIN_THREE, PIN_FOUR};
MembraneButton buttons[BUTTON_COUNT];

MembraneButton waterLevel;

wavTrigger wavTrig; // WAV Trigger control

void setup() {
  Serial.begin(57600);

  Serial.println("#Initializing WAV Trigger...");
  wavTrig.start();
  delay(10);

  // Send a stop-all command and reset the sample-rate offset, in case we have
  // reset while the WAV Trigger was already playing.
  wavTrigReset();

  Serial.println("#Setting up other hardware...");
  
  initPot();
  initButtons();
  waterLevel.configure(PIN_WATER_LEVEL);

  pinMode(PIN_PUMP, OUTPUT);
  digitalWrite(PIN_PUMP, LOW);

  Serial.println("#Setting up connection to cloud...");
  pinMode(PIN_TWISTED_GROUND_RX, OUTPUT);
  pinMode(PIN_TWISTED_GROUND_TX, OUTPUT);
  digitalWrite(PIN_TWISTED_GROUND_RX, LOW);
  digitalWrite(PIN_TWISTED_GROUND_TX, LOW);

  Serial.println("#Beginning...");
}

void loop() {
  checkSerial();
  checkButtons();
  checkPot();
  checkWaterLevel();
}

void checkWaterLevel() {
  // Water level high -> 1
  // Water level low -> 0
  uint8_t change = waterLevel.check();
  if (change != BUTTON_NOCHANGE) {
    Serial.print("$water ");
    if (change == BUTTON_DOWN) {
      Serial.println("1");
    } else if (change == BUTTON_UP) {
      Serial.println("0");
    } else {
      Serial.println("?");
    }
  }
}

void wavTrigReset() {
  wavTrig.stopAllTracks();
  wavTrig.samplerateOffset(0);
}
