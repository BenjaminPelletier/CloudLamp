const int SERIAL_BUFFER_LEN = 255;
char serial_buffer[SERIAL_BUFFER_LEN];
int serial_buffer_index = 0;

char gWTrigVersion[VERSION_STRING_LEN];

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
  int data = Serial.read();
  if (data == -1) return;
  char c = (char)data;
  if (c == '\r') {
    // Ignore line feeds
  } else if (c == '\n') {
    if (serial_buffer_index == 0) {
      Serial.println("!No command provided");
    } else if (serial_buffer[0] != '$') {
      Serial.print("!Command not prefixed with $ (instead ");
      Serial.print(serial_buffer[0]);
      Serial.print(' ');
      Serial.print((int)serial_buffer[0]);
      Serial.println(")");
    } else {
      serial_buffer[serial_buffer_index] = 0;
      String msg = String(serial_buffer + 1);
      runCommand(cmdOf(msg), argsOf(msg));
    }
    serial_buffer_index = 0;
  } else {
    if (serial_buffer_index < SERIAL_BUFFER_LEN - 1) {
      serial_buffer[serial_buffer_index++] = c;
    }
  }
}

void runCommand(String cmd, String args) {
  if (cmd == "echo") {
    Serial.println(args);
    
  } else if (cmd == "wav") {
    cmd = cmdOf(args);
    args = argsOf(args);
    if (cmd == "playpoly") {
      int track = args.toInt();
      wavTrig.trackPlayPoly(track);
      Serial.print("*wav playpoly ");
      Serial.println(track);

    } else if (cmd == "stopall") {
      wavTrig.stopAllTracks();
      Serial.println("*wav stopall");

    } else if (cmd == "stoptrack") {
      int track = args.toInt();
      wavTrig.trackStop(track);
      Serial.print("*wav stoptrack ");
      Serial.println(track);

    } else if (cmd == "loadtrack") {
      int track = args.toInt();
      wavTrig.trackLoad(track);
      Serial.print("*wav loadtrack ");
      Serial.println(track);

    } else if (cmd == "resumeall") {
      wavTrig.resumeAllInSync();
      Serial.println("*wav resumeall");

    } else if (cmd == "mastergain") {
      int gain = args.toInt();
      wavTrig.masterGain(gain);
      Serial.print("*wav mastergain ");
      Serial.println(gain);

    } else if (cmd == "trackgain") {
      String trackArg = cmdOf(args);
      String gainArg = argsOf(args);
      int track = trackArg.toInt();
      int gain = gainArg.toInt();
      wavTrig.trackGain(track, gain);
      Serial.print("*wav trackgain ");
      Serial.print(track);
      Serial.print(' ');
      Serial.println(gain);

    } else if (cmd == "trackcount?") {
      if (wavTrig.getVersion(gWTrigVersion, VERSION_STRING_LEN)) {
        Serial.print("*wav trackcount ");
        Serial.println(wavTrig.getNumTracks());
      } else {
        Serial.println("!WAV Trigger not available");
      }

    } else if (cmd == "version?") {
      if (wavTrig.getVersion(gWTrigVersion, VERSION_STRING_LEN)) {
        Serial.print("*wav version=");
        Serial.println(gWTrigVersion);
      } else {
        Serial.println("!WAV Trigger not available");
      }

    } else if (cmd == "istrackplaying?") {
      wavTrig.update();
      Serial.print("*wav trackplaying ");
      Serial.println(wavTrig.isTrackPlaying(args.toInt()) ? "true" : "false");

    } else if (cmd == "reset") {
      wavTrigReset();
      Serial.println("*wav reset");
      
    } else {
      Serial.print("!Unrecognized WAV command: ");
      Serial.println(cmd);
    }

  } else if (cmd == "pot?") {
    long value = analogRead(PIN_POT);
    int n = args.toInt();
    for (int i = 0; i < n; i++) {
      value += analogRead(PIN_POT);
    }
    if (n > 0) value /= n + 1;
    Serial.print("*pot ");
    Serial.println(value);

  } else if (cmd == "btn?") {
    int i = args.toInt();
    if (i < 0 || i >= BUTTON_COUNT) {
      Serial.print("!Button ");
      Serial.print(i);
      Serial.println(" does not exist");
    } else {
      Serial.print("*btn ");
      Serial.print(i);
      Serial.print(' ');
      Serial.println(buttons[i].currentState == LOW ? '1' : '0');
    }

  } else if (cmd == "water?") {
    Serial.print("*water ");
    Serial.println(waterLevel.currentState == LOW ? '1' : '0');

  } else if (cmd == "pump") {
    if (args == "on") {
      digitalWrite(PIN_PUMP, HIGH);
      Serial.println("*pump on");
    } else if (args == "off") {
      digitalWrite(PIN_PUMP, LOW);
      Serial.println("*pump off");
    } else {
      Serial.print("!Expected pump on or off, got ");
      Serial.println(args);
    }

  } else {
    Serial.print("!Unrecognized command: ");
    Serial.println(cmd);
  }
}
