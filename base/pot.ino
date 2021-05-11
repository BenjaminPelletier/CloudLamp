const int POT_CHANGE_THRESHOLD = 10; // [1 / (current ADC count value)] ADC counts
const unsigned long POT_UPDATE_PERIOD = 200; // milliseconds
const int POT_SAMPLES_AVERAGED = 100;

unsigned long pot_current;
unsigned long pot_last;
unsigned long pot_sum = 0;
int pot_sample_index;
unsigned long pot_last_change;
int pot_threshold;

void initPot() {
  pinMode(PIN_POT, INPUT);
  for (int i = 0; i < POT_SAMPLES_AVERAGED; i++) {
    pot_sum += analogRead(PIN_POT);
  }
  pot_last = pot_sum / POT_SAMPLES_AVERAGED;
  pot_current = pot_last;
  pot_last_change = millis();
  pot_threshold = 1 + pot_current / POT_CHANGE_THRESHOLD;
}

void checkPot() {
  pot_sum += analogRead(PIN_POT);
  pot_sample_index++;
  if (pot_sample_index >= POT_SAMPLES_AVERAGED) {
    pot_current = pot_sum / POT_SAMPLES_AVERAGED;
    pot_sum = 0;
    pot_sample_index = 0;
    if ((pot_current >= pot_last ? pot_current - pot_last : pot_last - pot_current) >= pot_threshold) {
      unsigned long t = millis();
      if (timeDelta(pot_last_change, t) >= POT_UPDATE_PERIOD) {
        Serial.print("$pot ");
        Serial.println(pot_current);
        pot_last = pot_current;
        pot_last_change = t;
        pot_threshold = 1 + pot_current / POT_CHANGE_THRESHOLD;
      }
    }
  }
}
