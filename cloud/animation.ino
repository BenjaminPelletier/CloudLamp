#include <FastLED.h>
#include "FastLED_RGBW.h"

void animateBlip(CRGB c) {
  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = c;
  }
  FastLED.show();
  
  delay(30);
  
  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

uint8_t inline tween(uint8_t v0b, uint8_t v1b, uint8_t f) {
  int v0 = v0b;
  int v1 = v1b;
  return (uint8_t)((v0 * (255 - f) + v1 * f) >> 8);
}

void showAnimationTween(uint8_t f) {
  for (int i=0; i<NUM_LEDS; i++) {
    leds[i].r = tween(animation_frame_0[i].r, animation_frame_1[i].r, f);
    leds[i].g = tween(animation_frame_0[i].g, animation_frame_1[i].g, f);
    leds[i].b = tween(animation_frame_0[i].b, animation_frame_1[i].b, f);
    leds[i].w = tween(animation_frame_0[i].w, animation_frame_1[i].w, f);
  }
  FastLED.show();
}
