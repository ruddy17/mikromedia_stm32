#ifndef MICROMEDIA_LCD_H
#define MICROMEDIA_LCD_H

#define LCD_WIDTH            (320)
#define LCD_HEIGHT           (240)
#define RGB(r,g,b)           (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

#define Set_Cs  GPIO_SetBits(GPIOE,GPIO_Pin_15);
#define Clr_Cs  GPIO_ResetBits(GPIOE,GPIO_Pin_15);

#define Set_Rs  GPIO_SetBits(GPIOE,GPIO_Pin_12);
#define Clr_Rs  GPIO_ResetBits(GPIOE,GPIO_Pin_12);

#define Set_nWr GPIO_SetBits(GPIOE,GPIO_Pin_11);
#define Clr_nWr GPIO_ResetBits(GPIOE,GPIO_Pin_11);

#define Set_nRd GPIO_SetBits(GPIOE,GPIO_Pin_10);
#define Clr_nRd GPIO_ResetBits(GPIOE,GPIO_Pin_10);

#define Set_Rst GPIO_SetBits(GPIOE,GPIO_Pin_8);
#define Clr_Rst GPIO_ResetBits(GPIOE,GPIO_Pin_8);

#define Lcd_Light_ON   GPIO_SetBits(GPIOE,GPIO_Pin_9);
#define Lcd_Light_OFF  GPIO_ResetBits(GPIOE,GPIO_Pin_9);

void LCD_Init(void);
uint8_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue);

void LCD_Clear(uint16_t Color);

#endif
