#include <FastLED.h>
#include "FastLED_RGBW.h"
#include "leds.h"
#include "atmosphere.h"

const int SUNSET_POT_MEDIUM = 750;
const float SUNSET_ANGLE_DISTANCE = 15.0f; // degrees
const float SUNSET_MINIMUM_DURATION = 15.0f * 1000.0f; // milliseconds
const float SUNSET_MEDIUM_DURATION = 10.0f * 60.0f * 1000.0f; // milliseconds
const float SUNSET_MAXIMUM_DURATION = 30.0f * 60.0f * 1000.0f; // milliseconds
float sunset_sun_angle;
unsigned long sunset_last_update;

void initSunset() {
  readPot();
  sunset_sun_angle = SUNSET_ANGLE_DISTANCE;
  sunset_last_update = millis();
}

void checkSunset() {
  unsigned long t = millis();
  int pot = getPot();
  float target_seconds;
  if (pot < SUNSET_POT_MEDIUM) {
    target_seconds = SUNSET_MAXIMUM_DURATION + (SUNSET_MEDIUM_DURATION - SUNSET_MAXIMUM_DURATION) * pot / SUNSET_POT_MEDIUM;
  } else {
    target_seconds = SUNSET_MEDIUM_DURATION + (SUNSET_MINIMUM_DURATION - SUNSET_MEDIUM_DURATION) * (pot - SUNSET_POT_MEDIUM) / (1024 - SUNSET_POT_MEDIUM);
  }
  float dEl = SUNSET_ANGLE_DISTANCE * (t - sunset_last_update) / target_seconds;
  sunset_last_update = t;
  sunset_sun_angle -= dEl;
  
  int az = 140;
  float el = sunset_sun_angle;
  if (sunset_sun_angle < -5.0f) {
    for (int i=0; i<NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
  } else {
    Vec3f sunDir = Vec3f(sin(radians(az)) * cos(radians(el)), sin(radians(el)), cos(radians(az)) * cos(radians(el)));
    Atmosphere atm = Atmosphere(sunDir);
  
    for (uint8_t zone=ZONE_TOP; zone<=ZONE_BOTTOM_BAND; zone++) {
      Vec3f cv = atm.computeIncidentLight(Vec3f(cos(radians((ZONE_BOTTOM_BAND - zone) * 5 + 2.5f)), sin(radians((ZONE_BOTTOM_BAND - zone) * 5 + 2.5f)), 0));
      cv.x *= cv.x;
      cv.y *= cv.y;
      cv.z *= cv.z;
      float white = 0;
      if (cv.x > 1.0f || cv.y > 1.0f || cv.z > 1.0f) {
        white = max(cv.x, max(cv.y, cv.z)) - 1.0f;
        cv.x -= white;
        cv.y -= white;
        cv.z -= white;
      }
      uint8_t r = cv.x * 255;
      uint8_t g = cv.y * 255;
      uint8_t b = cv.z * 255;
      uint8_t w = white * 255;
      setZone(zone, CRGBW(r, g, b, w));
    }
  }
  FastLED.show();
}
