#include "timedelta.h"

const unsigned long BUTTON_DEBOUNCE = 100; // milliseconds

void initButtons() {
  for (int b = 0; b < BUTTON_COUNT; b++) {
    buttons[b].configure(PIN_BUTTONS[b]);
  }
}

void MembraneButton::configure(int io_pin) {
  pin = io_pin;
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
  currentState = digitalRead(pin);
  lastChange = millis();
}

uint8_t MembraneButton::check() {
  int now = digitalRead(pin);
  if (now != currentState) {
    unsigned long t = millis();
    if (timeDelta(lastChange, t) > BUTTON_DEBOUNCE) {
      currentState = now;
      lastChange = t;
      return now == LOW ? BUTTON_DOWN : BUTTON_UP;
    }
  } else {
    return BUTTON_NOCHANGE;
  }
}

void checkButtons() {
  uint8_t change;
  for (int b = 0; b < BUTTON_COUNT; b++) {
    change = buttons[b].check();
    if (change != BUTTON_NOCHANGE) {
      Serial.print("$btn ");
      Serial.print(b);
      if (change == BUTTON_DOWN) {
        Serial.println(" 1");
      } else if (change == BUTTON_UP) {
        Serial.println(" 0");
      } else {
        Serial.println(" ?");
      }
    }
  }
}
