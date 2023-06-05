#ifndef __LPFILTER_H
#define __LPFILTER_H
#include <stm32f10x.h>

#define size_filter 5

uint8_t last_idex(uint8_t current);
uint8_t next_idex(uint8_t current);
float low_pass_filter(float input);
uint8_t is_peak(void);
#endif
