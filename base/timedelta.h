const unsigned long MAX_UL_VALUE = 0xFFFFFFFFul;

unsigned long timeDelta(unsigned long t0, unsigned long t1) {
  if (t1 >= t0) {
    return t1 - t0;
  } else {
    return t1 + (MAX_UL_VALUE - t0) + 1;
  }
}
