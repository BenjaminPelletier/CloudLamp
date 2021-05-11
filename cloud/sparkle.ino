unsigned long sparkle_last_update;
float sparkle_v[NUM_LEDS];
float sparkle_dvdt[NUM_LEDS];
float sparkle_sum;

void initSparkle() {
  sparkle_sum = 0.0f;
  for (int i=0; i<NUM_LEDS; i++) {
    sparkle_v[i] = random(0, 255) / 255.0f;
    sparkle_sum += sparkle_v[i];
    sparkle_dvdt[i] = (random(0, 512) - 255) / 255.0f;
  }
  sparkle_last_update = millis();
  checkSparkle();
}

void checkSparkle() {
  unsigned long t = millis();
  float dt = (float)timeDelta(sparkle_last_update, t) / 1000.0f;
  sparkle_last_update = t;

  float k = 5.0f - 4.5f * getPot() / 1024.0f;
  
  for (int i=0; i<NUM_LEDS; i++) {
    float v = sparkle_v[i] + dt * sparkle_dvdt[i];
    if (v > 1.0f) {
      v = 1.0f - (v - 1.0f);
      sparkle_dvdt[i] = -sparkle_dvdt[i];
    } else if (v < 0) {
      v = -v;
      sparkle_dvdt[i] = -sparkle_dvdt[i];
    }
    sparkle_sum += v - sparkle_v[i];
    sparkle_v[i] = v;
    leds[i].w = (uint8_t)(255.0f * pow(v, k));
  }
  FastLED.show();
}
