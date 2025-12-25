// 中景园电子 0.96寸OLED

#include "oled.h"
#include "delay.h"
#include "oledfont.h"
#include "stdlib.h"

// OLED的显存
// 存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127

void IIC_Start() {
    OLED_SCLK_Set();
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

void IIC_Stop() {
    OLED_SCLK_Set();
    //    OLED_SCLK_Clr();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}

void IIC_Wait_Ack() {
    OLED_SCLK_Set();
    OLED_SCLK_Clr();
}

/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte) {
    unsigned char i;
    unsigned char m, da;
    da = IIC_Byte;
    OLED_SCLK_Clr();
    for (i = 0; i < 8; i++) {
        m = da;
        //    OLED_SCLK_Clr();
        m = m & 0x80;
        if (m == 0x80) {
            OLED_SDIN_Set();
        } else
            OLED_SDIN_Clr();
        da = da << 1;
        OLED_SCLK_Set();
        OLED_SCLK_Clr();
    }
}

/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command) {
    IIC_Start();
    Write_IIC_Byte(0x78); // Slave address,SA0=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00); // write command
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
    IIC_Stop();
}

/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data) {
    IIC_Start();
    Write_IIC_Byte(0x78); // D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40); // write data
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data) {
    unsigned char m, n;
    for (m = 0; m < 8; m++) {
        Write_IIC_Command(0xb0 + m); // page0-page1
        Write_IIC_Command(0x00);     // low column start address
        Write_IIC_Command(0x10);     // high column start address
        for (n = 0; n < 128; n++) {
            Write_IIC_Data(fill_Data);
        }
    }
}

// 坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) {
    Write_IIC_Command(0xb0 + y);
    Write_IIC_Command(((x & 0xf0) >> 4) | 0x10);
    Write_IIC_Command((x & 0x0f));
}

// 开启OLED显示
void OLED_Display_On(void) {
    Write_IIC_Command(0X8D); // SET DCDC命令
    Write_IIC_Command(0X14); // DCDC ON
    Write_IIC_Command(0XAF); // DISPLAY ON
}

// 关闭OLED显示
void OLED_Display_Off(void) {
    Write_IIC_Command(0X8D); // SET DCDC命令
    Write_IIC_Command(0X10); // DCDC OFF
    Write_IIC_Command(0XAE); // DISPLAY OFF
}

// 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
        Write_IIC_Command(0xb0 + i); // 设置页地址（0~7）
        Write_IIC_Command(0x00);     // 设置显示位置—列低地址
        Write_IIC_Command(0x10);     // 设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            Write_IIC_Data(0);
    } // 更新显示
}

void OLED_On(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
        Write_IIC_Command(0xb0 + i); // 设置页地址（0~7）
        Write_IIC_Command(0x00);     // 设置显示位置—列低地址
        Write_IIC_Command(0x10);     // 设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            Write_IIC_Data(1);
    } // 更新显示
}

// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// mode:0,反白显示;1,正常显示
// size:选择字体 16/12
void OLED_ShowChar(u8 x, u8 y, char chr, u8 Char_Size) {
    unsigned char c = 0, i = 0;
    c = chr - ' '; // 得到偏移后的值
    if (x >= OLED_WIDTH) {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16) {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            Write_IIC_Data(F8X16[c * 16 + i]);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            Write_IIC_Data(F8X16[c * 16 + i + 8]);
    } else {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++)
            Write_IIC_Data(F6x8[c][i]);
    }
}

// m^n函数
u32 oled_pow(u8 m, u8 n) {
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}

// 显示2个数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:模式    0,填充模式;1,叠加模式
// num:数值(0~4294967295);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2) {
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            } else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
    }
}

// 显示一个字符号串
void OLED_ShowString(u8 x, u8 y, const char *str, u8 Char_Size) {
    while (*str) {
        OLED_ShowChar(x, y, *str, Char_Size);
        x += 8;
        if (x > 120) {
            x = 0;
            y += 2;
        }
        str++;
    }
}

// 显示汉字
void OLED_ShowCHinese(u8 x, u8 y, u8 no) {
    u8 t, adder = 0;
    OLED_Set_Pos(x, y);
    for (t = 0; t < 16; t++) {
        Write_IIC_Data(Hzk[2 * no][t]);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for (t = 0; t < 16; t++) {
        Write_IIC_Data(Hzk[2 * no + 1][t]);
        adder += 1;
    }
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[]) {
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++) {
        OLED_Set_Pos(x0, y);
        for (x = x0; x < x1; x++) {
            Write_IIC_Data(BMP[j++]);
        }
    }
}

// 初始化SSD1306
void OLED_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // 使能PD端口时钟

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // PD6,PD7推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      // 速度50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);                 // 初始化GPIOD6,7
    GPIO_SetBits(GPIOD, GPIO_Pin_6 | GPIO_Pin_7);          // PD6,PD7 输出高

    delay_ms(50);

    Write_IIC_Command(0xAE); //--display off
    Write_IIC_Command(0x00); //---set low column address
    Write_IIC_Command(0x10); //---set high column address
    Write_IIC_Command(0x40); //--set start line address
    Write_IIC_Command(0xB0); //--set page address
    Write_IIC_Command(0x81); // contract control
    Write_IIC_Command(0xFF); //--128
    Write_IIC_Command(0xA1); // set segment remap
    Write_IIC_Command(0xA6); //--normal / reverse
    Write_IIC_Command(0xA8); //--set multiplex ratio(1 to 64)
    Write_IIC_Command(0x3F); //--1/32 duty
    Write_IIC_Command(0xC8); // Com scan direction
    Write_IIC_Command(0xD3); //-set display offset
    Write_IIC_Command(0x00); //
    Write_IIC_Command(0xD5); // set osc division
    Write_IIC_Command(0x80); //
    Write_IIC_Command(0xD8); // set area color mode off
    Write_IIC_Command(0x05); //
    Write_IIC_Command(0xD9); // Set Pre-Charge Period
    Write_IIC_Command(0xF1); //
    Write_IIC_Command(0xDA); // set com pin configuartion
    Write_IIC_Command(0x12); //
    Write_IIC_Command(0xDB); // set Vcomh
    Write_IIC_Command(0x30); //
    Write_IIC_Command(0x8D); // set charge pump enable
    Write_IIC_Command(0x14); //
    Write_IIC_Command(0xAF); //--turn on oled panel
    
    delay_ms(50);
    OLED_Clear();
}
