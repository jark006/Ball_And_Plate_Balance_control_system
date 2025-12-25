#include "delay.h"

static u32 fac_us = 0;  // us延时倍乘数
static u32 fac_ms = 0;  // ms延时倍乘数

extern u32 SystemCoreClock;

// 初始化延迟函数
void Delay_Init() {
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // 选择外部时钟 HCLK/8
    fac_us = SystemCoreClock / 8000000;                   // 为系统时钟的1/8
    fac_ms = fac_us * 1000;
}

// 微秒延时
void delay_us(u32 nus) {
    u32 temp;
    SysTick->LOAD = nus * fac_us;             // 时间加载
    SysTick->VAL = 0x00;                      // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒数
    do {
        temp = SysTick->CTRL;
    } while (
        temp & 0x01 &&
        !(temp & (1 << 16))); // 等待时间到达 //bit16置一说明时间到，跳出循环
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    SysTick->VAL = 0X00;                       // 清空计数器
}

// 毫秒延时 最大限制 1864ms
// SysTick->LOAD为24位寄存器,8分频, 所以最大延时为:
// nms < (2^24) / (SYSCLK/8) * 1000ms
// 对72M条件下, nms < 1864ms
static void delay_ms_limit(u16 nms) {
    u32 temp;
    SysTick->LOAD = nms * fac_ms;             // 时间加载(SysTick->LOAD为24bit)
    SysTick->VAL = 0x00;                      // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒数
    do {
        temp = SysTick->CTRL;
    } while (temp & 0x01 && !(temp & (1 << 16))); // 等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    SysTick->VAL = 0X00;                       // 清空计数器
}

// 毫秒延时
void delay_ms(u32 ms) {
    u32 ms1024 = ms >> 10;
    u16 msLess = ms & 0x3ff;
    while (ms1024--)
        delay_ms_limit(1024);
    if (msLess)
        delay_ms_limit(msLess);
}

// 秒延时
void delay_sec(u32 sec) {
    while (sec--)
        delay_ms_limit(1000);
}
