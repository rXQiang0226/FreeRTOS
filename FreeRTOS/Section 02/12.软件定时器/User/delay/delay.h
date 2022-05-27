

#ifndef _DELAY_H
#define _DELAY_H

#include "system.h"

void Delay_Init(uint8_t SYSCLK);
void Delay_ms(uint16_t nms);
void Delay_us(uint32_t nus);

#endif
