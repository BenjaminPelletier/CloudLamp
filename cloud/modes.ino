void changeMode(uint8_t new_mode) {
  if (current_mode == new_mode) {
    current_mode = MODE_OFF;
    animateBlip(CRGB::Red);
  } else {
    current_mode = new_mode;
    if (new_mode == MODE_STORM) {
      initStorm();
      animateBlip(CRGB::White);
    } else if (new_mode == MODE_SPARKLE) {
      initSparkle();
      animateBlip(CRGB::Green);
    } else if (new_mode == MODE_SUNSET) {
      initSunset();
      animateBlip(CRGB::Blue);
    }
  }
}
