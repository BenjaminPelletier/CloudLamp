#include <FastLED.h>
#include "FastLED_RGBW.h"
#include "leds.h"

void setZone(uint8_t zone, CRGB c) {
  CRGBW c2;
  c2.r = c.r;
  c2.g = c.g;
  c2.b = c.b;
  setZone(zone, c2);
}

void setZone(uint8_t zone, CRGBW c) {
  if (zone == ZONE_TOP) {
    for (int i=0; i<25; i++) {
      leds[i] = c;
    }
    for (int i=129; i<140; i++) {
      leds[i] = c;
    }
  } else if (zone == ZONE_TOP_BAND) {
    for (int i=25; i<64; i++) {
      leds[i] = c;
    }
    leds[128] = c;
    leds[140] = c;
  } else if (zone == ZONE_TOP_SIDES) {
    for (int i=64; i<68; i++) {
      leds[i] = c;
    }
    leds[126] = c;
    leds[127] = c;
    leds[141] = c;
    leds[142] = c;
    leds[148] = c;
    leds[149] = c;
  } else if (zone == ZONE_MIDDLE_BAND) {
    for (int i=68; i<91; i++) {
      leds[i] = c;
    }
    leds[125] = c;
    leds[143] = c;
    leds[147] = c;
  } else if (zone == ZONE_BOTTOM_SIDES) {
    for (int i=91; i<98; i++) {
      leds[i] = c;
    }
    leds[124] = c;
    leds[145] = c;
    leds[146] = c;
  } else if (zone == ZONE_BOTTOM_BAND) {
    for (int i=98; i<123; i++) {
      leds[i] = c;
    }
    leds[144] = c;
  }
}
