#ifndef LIGHTS_H
#define LIGHTS_H
#include <stdint.h>
void static_color(uint32_t color);
void brightness(uint8_t value);
void breathe(uint32_t color);
void spectrum(uint16_t duration);
void wave(uint32_t color);
void rainbow(uint16_t duration);
void back_and_forth(uint32_t color);
void example_spectrum(uint16_t duration);
void defaultblue(uint32_t color);
void example_breathe(uint16_t duration, uint32_t color);

#endif // LIGHTS_H
