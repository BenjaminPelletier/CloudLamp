#include "base.h"

const int SERIAL_BUFFER_LEN = 255;
char serial_buffer[SERIAL_BUFFER_LEN];
int serial_buffer_index = 0;

int current_pot_value = 0;
int current_water_value = 0;

void initBase() {
  Serial2.begin(57600);
  readPot();
  readWater();
}

void playThunder(int index) {
  Serial2.print("$wav playpoly ");
  Serial2.println(index);
}

void readPot() {
  Serial2.println("$pot?");
}

void readWater() {
  Serial2.println("$water?");
}

int getPot() {
  return current_pot_value;
}

// Communication follows this scheme:
// $command arg1 arg2
// $unpromptedevent arg1 arg2
// *response arg1 arg2
// !Some kind of error
// #General log message

String cmdOf(String msg) {
  int s = msg.indexOf(' ');
  if (s == -1) s = msg.length();
  return msg.substring(0, s);
}

String argsOf(String msg) {
  int s = msg.indexOf(' ');
  if (s == -1) s = msg.length();
  return s == msg.length() ? "" : msg.substring(s + 1);
}

void checkSerial() {
  int data = Serial2.read();
  if (data == -1) return;
  char c = (char)data;
  if (c == '\r') {
    // Ignore line feeds
  } else if (c == '\n') {
    if (serial_buffer_index == 0) {
      Serial.println("!Base: No command provided");
    } else if (serial_buffer[0] == '!') {
      Serial.print("!Base: Error: ");
      serial_buffer[serial_buffer_index] = 0;
      Serial.println(serial_buffer);
    } else if (serial_buffer[0] == '#') {
      Serial.print("#Base: ");
      serial_buffer[serial_buffer_index] = 0;
      Serial.println(serial_buffer);
    } else if (serial_buffer[0] == '$' || serial_buffer[0] == '*') {
      serial_buffer[serial_buffer_index] = 0;
      String msg = String(serial_buffer + 1);
      processMessage(cmdOf(msg), argsOf(msg));
    } else {
      Serial.print("!Base response prefix not one of !#$* (instead ");
      Serial.print(serial_buffer[0]);
      Serial.print(' ');
      Serial.print((int)serial_buffer[0]);
      Serial.print("): ");
      serial_buffer[serial_buffer_index] = 0;
      Serial.println(serial_buffer);
    }
    serial_buffer_index = 0;
  } else {
    if (serial_buffer_index < SERIAL_BUFFER_LEN - 1) {
      serial_buffer[serial_buffer_index++] = c;
    }
  }
}

void processMessage(String cmd, String args) {
  if (cmd == "wav") {
    cmd = cmdOf(args);
    args = argsOf(args);
    if (cmd == "playpoly") {
      int track = args.toInt();
      Serial.print("*wav playpoly ");
      Serial.println(track);

    } else if (cmd == "stopall") {
      Serial.println("*wav stopall");

    } else if (cmd == "stoptrack") {
      int track = args.toInt();
      Serial.print("*wav stoptrack ");
      Serial.println(track);

    } else if (cmd == "loadtrack") {
      int track = args.toInt();
      Serial.print("*wav loadtrack ");
      Serial.println(track);

    } else if (cmd == "resumeall") {
      Serial.println("*wav resumeall");

    } else if (cmd == "mastergain") {
      int gain = args.toInt();
      Serial.print("*wav mastergain ");
      Serial.println(gain);

    } else if (cmd == "trackgain") {
      String trackArg = cmdOf(args);
      String gainArg = argsOf(args);
      int track = trackArg.toInt();
      int gain = gainArg.toInt();
      Serial.print("*wav trackgain ");
      Serial.print(track);
      Serial.print(' ');
      Serial.println(gain);

    } else if (cmd == "trackcount") {
      int trackCount = args.toInt();;
      Serial.print("*wav trackcount ");
      Serial.println(trackCount);

    } else if (cmd == "version") {
      Serial.print("*wav version ");
      Serial.println(args);

    } else if (cmd == "trackplaying") {
      Serial.print("*wav trackplaying ");
      Serial.println(args);

    } else if (cmd == "reset") {
      Serial.println("*wav reset");
      
    } else {
      Serial.print("!Unrecognized WAV message from Base: ");
      Serial.println(cmd);
    }

  } else if (cmd == "pot") {
    current_pot_value = args.toInt();
    Serial.print("*pot ");
    Serial.println(current_pot_value);

  } else if (cmd == "btn") {
    String btnArg = cmdOf(args);
    String stateArg = argsOf(args);
    int btn = btnArg.toInt();
    int state = stateArg.toInt();
    Serial.print("*btn ");
    Serial.print(btn);
    Serial.print(' ');
    Serial.println(state);

    // TODO: Move button processing logic to more reasonable location
    if (btn == 0 && state == 0) {
      pump_requested = !pump_requested;
      if (pump_requested) {
        readWater();
        if (current_water_value > 0) {
          Serial2.println("$pump on");
        } else {
          animateBlip(CRGB::Yellow);
        }
      } else {
        Serial2.println("$pump off");
      }
    } else if (btn == 1 && state == 0) {
      changeMode(MODE_STORM);
    } else if (btn == 2 && state == 0) {
      changeMode(MODE_SUNSET);
    } else if (btn == 3 && state == 0) {
      changeMode(MODE_SPARKLE);
    }

  } else if (cmd == "water") {
    int state = args.toInt();
    Serial.print("*water ");
    Serial.println(state);
    if (pump_requested && state == 1) {
      Serial2.println("$pump on");
    }
    if (state == 0) {
      Serial2.println("$pump off");
    }

  } else if (cmd == "pump") {
    if (args == "on") {
      Serial.println("*pump on");
    } else if (args == "off") {
      Serial.println("*pump off");
    } else {
      Serial.print("!Base indicated pump in unknown state: ");
      Serial.println(args);
    }

  } else if (cmd == "select") {
    int n = args.toInt();
    for (int i=0; i<NUM_LEDS; i++) {
      leds[i] = i == n ? CRGB::White : CRGB::Black;
    }
    FastLED.show();
    Serial.print("*select ");
    Serial.println(n);

  } else {
    Serial.print("!Unrecognized message from Base: ");
    Serial.println(cmd);
  }
}
