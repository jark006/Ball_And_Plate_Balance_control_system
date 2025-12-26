// 中景园电子 0.96寸OLED

#ifndef __OLED_H
#define __OLED_H
#include "stdlib.h"
#include "sys.h"

#define OLED_WIDTH  128
#define OLED_HEIGHT 64

//-----------------OLED IIC端口定义----------------
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOD, GPIO_Pin_6)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOD, GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD, GPIO_Pin_7)
#define OLED_SDIN_Set() GPIO_SetBits(GPIOD, GPIO_Pin_7)

// OLED控制用函数
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x, u8 y, char chr, u8 Char_Size);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);
void OLED_ShowString(u8 x, u8 y, const char* str, u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x, u8 y, u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
    unsigned char y1, unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
#endif
