#include <FastLED.h>
#include "FastLED_RGBW.h"
#include "thunder_sounds.h"

const int LIGHTNING_DURATION = 15; // milliseconds

class BoltState {
  private:
    int boltIndex;
    unsigned long tStarted;
    unsigned long dtThunder;
    unsigned int flashIndex;
    bool flashOn;
    bool playedThunder;

  public:
    bool active = false;
    void init(int index, unsigned long t);
    void update();
};

const int MAX_BOLTS = 5;
BoltState bolts[MAX_BOLTS];
unsigned long last_bolt = 0;
unsigned long current_bolt_delay = 0;

const int BOLT_CHECK_INTERVAL = 500; // milliseconds
unsigned long tNextBoltCheck = 0;
bool boltTriggering = false;

void initStorm() {
  last_bolt = millis();
  current_bolt_delay = 0;
}

void checkStorm() {
  checkNewBolt();
  updateBolts();
}

void checkNewBolt() {
  FastLED.show();
  unsigned long t = millis();
  if (timePassed(t, tNextBoltCheck)) {
    tNextBoltCheck = t + BOLT_CHECK_INTERVAL;
    if (boltTriggering) {
      Serial.println("bolt!");
      playBolt((unsigned long)analogRandom.get() * 2000 / 0x10000ul);
      readPot();
      boltTriggering = false;
    } else {
      int pot = getPot();
      if (pot > 0) {
        float p = (float)analogRandom.get() / 65536.0f;
        float mu = 1000.0f * ((14.0f * (1024 - pot)) / 1024 + 1.0f);
        float t_bolt = -log(1 - p) * mu;
        //Serial.print("p="); Serial.print(p); Serial.print(", mu="); Serial.print(mu); Serial.print(", t_bolt="); Serial.print(t_bolt);
        if (t_bolt < BOLT_CHECK_INTERVAL) {
          int dt = (int)t_bolt;
          tNextBoltCheck += dt;
          boltTriggering = true;
          //Serial.print(" **trig "); Serial.print(dt);
        }
        //Serial.println();
      }
    }
  }
}

void playBolt(int index) {
  for (int i = 0; i < MAX_BOLTS; i++) {
    if (!bolts[i].active) {
      //Serial.print("  Init'ing bolt ");
      //Serial.println(i);
      bolts[i].init(index, millis());
      break;
    }
  }
}

void updateBolts() {
  unsigned long t = millis();
  
  for (int i = 0; i < MAX_BOLTS; i++) {
    if (bolts[i].active) {
      bolts[i].update();
    }
  }
}

void BoltState::init(int index, unsigned long t) {
  active = true;
  boltIndex = index;
  tStarted = t;
  flashIndex = thunder_offsets[boltIndex];
  dtThunder = thunder_delays[flashIndex];
  flashOn = false;
  playedThunder = false;
}

void BoltState::update() {
  if (!playedThunder && millis() >= tStarted + dtThunder) {
    playThunder(boltIndex);
    playedThunder = true;
  }
  
  while (flashIndex < thunder_offsets[boltIndex + 1]) {
    unsigned long tEvent = tStarted + thunder_delays[flashIndex] - dtThunder;
    if (!flashOn) {
      if (timePassed(millis(), tEvent)) {
        // Turn on LEDs
        //Serial.print("on ");
        uint8_t v = thunder_intensities[flashIndex];
        uint16_t r;
        for (int i = 0; i < NUM_LEDS; i++) {
          r = analogRandom.get();
          if ((r & 0xFF) <= v) {
            leds[i].w = (uint8_t)(r >> 8);
          }
        }
        FastLED.show();
  
        flashOn = true;
      }
    }
    
    if (flashOn) {
      tEvent += LIGHTNING_DURATION;
      if (timePassed(millis(), tEvent)) {
        //Serial.print("off ");
        // Turn off LEDs
        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Black;
        }
        FastLED.show();
  
        flashIndex++;
        flashOn = false;
        continue;
      }
    }

    break;
  }

  if (playedThunder && flashIndex >= thunder_offsets[boltIndex + 1]) {
    active = false;
  }
}
