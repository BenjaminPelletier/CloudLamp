#ifndef Stats_h
#define Stats_h

const int BITS_PER_RANDOM_VALUE = 16;
const int RANDOM_BUFFER_SIZE = 10;

class ADCRandom {
  private:
    uint16_t bank[RANDOM_BUFFER_SIZE];
    int write_index;
    int read_index;
    uint8_t bit_index;
    bool buffer_exhausted;

  public:
    void init();
    void update();
    uint16_t get();
};

#endif
