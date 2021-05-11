#ifndef Base_h
#define Base_h

const uint8_t BUTTON_COUNT = 4;
const uint8_t BUTTON_DOWN = LOW;
const uint8_t BUTTON_UP = HIGH;

class LampBase {
  private:
    uint8_t buttons[BUTTON_COUNT];
    int write_index;
    int read_index;
    uint8_t bit_index;

  public:
    void init();
    void update();
    uint16_t get();
};

#endif
