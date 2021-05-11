#include <FastLED.h>
#include "FastLED_RGBW.h"
#include "stats.h"
#include "timedelta.h"
#include "base.h"

#include <driver/adc.h>

// Board: ESP32 Dev Module

#define NUM_LEDS 150
#define LED_DATA_PIN 4

LampBase base;

ADCRandom analogRandom;
// Note: ADCRandom uses GPIO pins 36 + 39 for hall_sensor_read()

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];
CRGBW animation_frame_0[NUM_LEDS];
CRGBW animation_frame_1[NUM_LEDS];

unsigned long tNextPotCheck = 0;

const uint8_t MODE_OFF = 0;
const uint8_t MODE_STORM = 1;
const uint8_t MODE_SPARKLE = 2;
const uint8_t MODE_SUNSET = 3;
uint8_t current_mode = MODE_OFF;

bool pump_requested = false;

void setup() {
  Serial.begin(57600);

  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  Serial.println();

  Serial.println("#Initializing connection to base...");
  initBase();
  
  Serial.println("#Initializing random...");
  analogRandom.init();

  Serial.println("#Initializing LEDs...");

  FastLED.addLeds<WS2812B, LED_DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
  FastLED.show();

  Serial.println("#Init complete.");
}

void loop() {
  analogRandom.update();
  if (current_mode == MODE_STORM) {
    checkStorm();
  } else if (current_mode == MODE_SPARKLE) {
    checkSparkle();
  } else if (current_mode == MODE_SUNSET) {
    checkSunset();
  }
  checkSerial();
  checkDebug();
  checkPot();
}

void checkPot() {
  if (timePassed(millis(), tNextPotCheck)) {
    readPot();
    tNextPotCheck += 5000;
  }
}
