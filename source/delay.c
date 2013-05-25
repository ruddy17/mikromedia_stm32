/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"

__IO uint32_t TimingDelayus1;
__IO uint32_t TimingDelayus2;
__IO uint32_t fps_time;
__IO uint32_t fps_update;

void FPS_Increment(void)
{
	fps_time++;
	fps_update++;
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay10us_Decrement(void)
{
  if (TimingDelayus1 >= 10)
  {
    TimingDelayus1 -= 10;
  } else if (TimingDelayus1 < 10) {
	TimingDelayus1 = 0;
  }

  if (TimingDelayus2 >= 10)
  {
    TimingDelayus2 -= 10;
  } else if (TimingDelayus2 < 10) {
	TimingDelayus2 = 0;
  }

}

void Delay_us(__IO uint32_t nTime)
{
  TimingDelayus1 = nTime;

  while(TimingDelayus1 != 0);
}

void Delay_Ms(__IO uint32_t nTime)
{
  TimingDelayus2 = nTime*1000;

  while(TimingDelayus2 != 0);
}

//void Delay_Ms(__IO uint32_t nTime)
//{
//  u32 time = nTime;
//  u32 i;
//
//  while(time != 0)
//  {
//	  for(i=0;i<14000;i++);
//	  time--;
//  }
//}

void SysTick_Handler(void)
{
	TimingDelay10us_Decrement();
	FPS_Increment();
}

