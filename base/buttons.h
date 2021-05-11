#ifndef Buttons_h
#define Buttons_h

const uint8_t BUTTON_NOCHANGE = 0;
const uint8_t BUTTON_DOWN = 1;
const uint8_t BUTTON_UP = 2;

class MembraneButton {
  private:
    int pin;
    unsigned long lastChange;

  public:
    int currentState;
    
    void configure(int pin_a0);
    uint8_t check();
};

#endif
