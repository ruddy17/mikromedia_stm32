#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f4xx.h"

extern __IO uint32_t TimingDelayMs;
extern __IO uint32_t TimingDelayus;
extern __IO uint32_t fps_time;
extern __IO uint32_t fps_update;

void TimingDelay10us_Decrement(void);
void TimingDelayMs_Decrement(void);
void FPS_Increment(void);

void Delay_Ms(__IO uint32_t nTime);
void Delay_us(__IO uint32_t nTime);

#endif /* __DELAY_H__ */
