#ifndef LIGHTS_H
#define LIGHTS_H
#include <stdint.h>
void wave(uint32_t color);
void rainbow(uint16_t duration);
void back_and_forth(uint32_t color);
void example_spectrum(uint16_t duration);
void example_static(uint32_t color);
void example_breathe(uint16_t duration, uint32_t color);

#endif // LIGHTS_H