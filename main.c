
#include "stm32f4xx.h"
#include "mikromedia.h"
#include "mikromedia_lcd.h"
#include "stm32f4xx_rcc.h"
#include "core_cm4.h"
#include "misc.h"

u16 Rainbow(void);


int main(void)
{

	SystemInit();
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100000);

	LCD_Init();
	u32 i;

	while(1)
	{
		LCD_Clear(Rainbow());

		//		for(i=0;i<245;i+=7)
		//		{
		//			LCD_Clear(RGB(255-i,i,0));
		//		}
		//
		//		for(i=0;i<245;i+=7)
		//		{
		//			LCD_Clear(RGB(0,255-i,i));
		//		}
		//
		//		for(i=0;i<245;i+=5)
		//		{
		//			LCD_Clear(RGB(i,0,255-i));
		//		}

		//		LCD_Clear(0xFF00);
		//		Delay_Ms(1000);
		//		LCD_Clear(RGB(255,0,255));
		//		Delay_Ms(1000);
	}
}

u16 Rainbow(void)
{
	static u8 r=255,g=0,b=255,mode=0, n;
	for(n=0;n<4;n++)
	{
		switch (mode)
		{
		case 0:
			if((--b) == 0)
			{
				mode++;
			}
			break;

		case 1:
			if((++g) == 255)
			{
				mode++;
			}
			break;

		case 2:
			if((--r) == 0)
			{
				mode++;
			}
			break;

		case 3:
			if((++b) == 255)
			{
				mode++;
			}
			break;

		case 4:
			if((--g) == 0)
			{
				mode++;
			}
			break;

		case 5:
			if((++r) == 255)
			{
				mode=0;
			}
			break;
		}
	}

	return RGB(r,g,b);
}

