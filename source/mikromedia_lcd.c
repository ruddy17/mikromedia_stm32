#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "mikromedia_lcd.h"

void LCD_Area(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);

void LCD_Bus_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// GPIO PE0..7 (LCD DB10..17)
	// GPIO PE8 (LCD RST)
	// GPIO PE9 (LCD Backlight)
	// GPIO PE10(LCD nRD)
	// GPIO PE11(LCD nWR)
	// GPIO PE12(LCD RS)
	// GPIO PE15(LCD CS)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6
			| GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void LCD_BusLinesDirection(uint8_t out)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if (out)
	{
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	}
	else
	{
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	}
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void LCD_Init(void)
{
	LCD_Bus_Init();

	Set_Cs;
	Set_Rs;
	Set_nWr;
	Set_nRd;
	Clr_Rst;
	Delay_Ms(10);
	Set_Rst;
	Delay_Ms(10);
	Lcd_Light_ON;

	//driving ability
	LCD_WriteReg(0xEA, 0x0000);
	LCD_WriteReg(0xEB, 0x0020);
	LCD_WriteReg(0xEC, 0x000C);
	LCD_WriteReg(0xED, 0x00C4);
	LCD_WriteReg(0xE8, 0x0040);
	LCD_WriteReg(0xE9, 0x0038);
	LCD_WriteReg(0xF1, 0x0001);
	LCD_WriteReg(0xF2, 0x0010);
	LCD_WriteReg(0x27, 0x00A3);

	//power voltage
	LCD_WriteReg(0x1B, 0x001B);
	LCD_WriteReg(0x1A, 0x0001);
	LCD_WriteReg(0x24, 0x002F);
	LCD_WriteReg(0x25, 0x0057);

	//VCOM offset
	LCD_WriteReg(0x23, 0x008D); //for flicker adjust

	//power on
	LCD_WriteReg(0x18, 0x0036);
	LCD_WriteReg(0x19, 0x0001); //start osc
	LCD_WriteReg(0x01, 0x0000); //wakeup
	LCD_WriteReg(0x1F, 0x0088);
	Delay_Ms(5);
	LCD_WriteReg(0x1F, 0x0080);
	Delay_Ms(5);
	LCD_WriteReg(0x1F, 0x0090);
	Delay_Ms(5);
	LCD_WriteReg(0x1F, 0x00D0);
	Delay_Ms(5);

	//color selection
	LCD_WriteReg(0x17, 0x0005); //0x0005=65k, 0x0006=262k

	//panel characteristic
	LCD_WriteReg(0x36, 0x0000);

	//display on
	LCD_WriteReg(0x28, 0x0038);
	Delay_Ms(40);
	LCD_WriteReg(0x28, 0x003C);

	//display options
	LCD_WriteReg(0x16, 0x00A8); //MY=1 MX=0 MV=1 ML=0 BGR=1

	LCD_Area(0, 0, (LCD_WIDTH-1), (LCD_HEIGHT-1));
}

void LCD_BusLinesWrite(uint8_t data)
{
	GPIOE->BSRRH = 0x00FF;
	GPIOE->BSRRL = data;
}

uint8_t LCD_BusLinesRead(void)
{
	uint8_t tmp;
	LCD_BusLinesDirection(0);
	tmp = (GPIOE->IDR) & 0xFF;
	LCD_BusLinesDirection(1);

	return tmp;
}

void LCD_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue)
{
	Clr_Cs;
	Clr_Rs;
	Set_nRd;
	LCD_BusLinesWrite(LCD_Reg);
	Clr_nWr;
	Set_nWr;
	Set_Rs;
	LCD_BusLinesWrite(LCD_RegValue);
	Clr_nWr;
	Set_nWr;
	Set_Cs;
}

uint8_t LCD_ReadReg(uint8_t LCD_Reg)
{
	uint8_t tmp;
	Clr_Cs;
	Clr_Rs;
	Set_nRd;
	LCD_BusLinesWrite(LCD_Reg);
	Clr_nWr;
	Set_nWr;
	Set_Rs;
	Clr_nRd;
	tmp = LCD_BusLinesRead();
	Set_nRd;
	Set_Cs;
	return tmp;
}

/* @brief  Prepare to write to the LCD RAM */
void LCD_WriteRAM_Prepare(void)
{
	Clr_Cs;
	Clr_Rs;
	Set_nRd;
	LCD_BusLinesWrite(0x22); /* Select GRAM Reg */
	Clr_nWr;
	Set_nWr;
	Set_Rs;
}

/* @brief  Writes to the LCD RAM.
 * @param  RGB_Code: the pixel color in RGB mode (5-6-5) */
void LCD_WriteRAM(uint16_t RGB_Code)
{
	LCD_BusLinesWrite(RGB_Code >> 8);
	Clr_nWr;
	Set_nWr;
	LCD_BusLinesWrite(RGB_Code & 0xFF);
	Clr_nWr;
	Set_nWr;
}

/* @brief Release LCD by setting /CS */
void LCD_Release(void)
{
	Set_Cs;
}

void LCD_Area(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
	LCD_WriteReg(0x03, (x0>>0)); //set x0
	LCD_WriteReg(0x02, (x0>>8)); //set x0
	LCD_WriteReg(0x05, (x1>>0)); //set x1
	LCD_WriteReg(0x04, (x1>>8)); //set x1
	LCD_WriteReg(0x07, (y0>>0)); //set y0
	LCD_WriteReg(0x06, (y0>>8)); //set y0
	LCD_WriteReg(0x09, (y1>>0)); //set y1
	LCD_WriteReg(0x08, (y1>>8)); //set y1
	return;
}

/* @brief  Clears the hole LCD.
 * @param  Color: the color of the background */
void LCD_Clear(uint16_t Color)
{
	uint32_t index;

	LCD_Area(0,0,(LCD_WIDTH-1), (LCD_HEIGHT-1));
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	for (index = 0; index < (240 * 320L); index++)
	{
		LCD_WriteRAM(Color);
	}
	LCD_Release();
}
