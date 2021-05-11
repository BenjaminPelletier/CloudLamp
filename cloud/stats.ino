#include <driver/adc.h>
#include <math.h>
#include "stats.h"

void ADCRandom::init() {
  adc1_config_width(ADC_WIDTH_BIT_12);

  write_index = 0;
  read_index = -1;
  bit_index = 0;
  buffer_exhausted = false;
  bool first_byte_initialized = false;

  int i = 0;
  
  while (i < 10000) {
    update();
    if (first_byte_initialized && write_index == read_index) {
      break;
    } else {
      if (write_index > 0) {
        first_byte_initialized = true;
        read_index = 0;
      }
    }
    i++;
  }
}

void ADCRandom::update() {
  int adc = hall_sensor_read();
  
  if (adc == 0 || adc == 4095) {
    // Ignore measurements without random content
    return;
  }
  if (write_index == read_index) {
    // Buffer is full
    return;
  }
  
  bank[write_index] <<= 1;
  if (adc & 1 > 0) {
    bank[write_index] |= 1;
  }
  bit_index++;
  if (bit_index >= BITS_PER_RANDOM_VALUE) {
    write_index++;
    buffer_exhausted = false;
    if (write_index >= RANDOM_BUFFER_SIZE) {
      write_index = 0;
    }
    bit_index = 0;
  }
}

uint16_t ADCRandom::get() {
  int next_index = read_index + 1;
  if (next_index >= RANDOM_BUFFER_SIZE) {
    next_index = 0;
  }
  if (next_index == write_index) {
    // We have run out of ADC-generated random numbers; pseudo-randomly mutate last random number in place
    if (!buffer_exhausted) {
      randomSeed(bank[read_index]);
      buffer_exhausted = true;
    }
    bank[read_index] = random(0, 0x10000);
    return bank[read_index];
  } else {
    uint16_t result = bank[read_index];
    read_index = next_index;
    if (read_index >= RANDOM_BUFFER_SIZE) {
      read_index = 0;
    }
    return result;
  }
}

unsigned long poissonDelay(double mean) {
  double p = ((double)(analogRandom.get())) / (double)(0x10000);
  return (unsigned long)(-mean * log(1 - p) * 1000);
}
