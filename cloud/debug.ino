const int DEBUG_BUFFER_LEN = 255;
char debug_buffer[DEBUG_BUFFER_LEN];
int debug_buffer_index = 0;

void checkDebug() {
  int c = Serial.read();
  if (c > -1) {
    if ((char)c == '\n') {
      debug_buffer[debug_buffer_index] = 0;
      Serial2.println(debug_buffer);
      debug_buffer_index = 0;
    } else {
      debug_buffer[debug_buffer_index] = (char)c;
      if (debug_buffer_index < DEBUG_BUFFER_LEN) debug_buffer_index++;
    }
  }
}
