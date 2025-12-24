#include "timer.h"

// 通用定时器2初始化
// arr：自动重装值。
// psc：时钟预分频数
void TIM2_Init(u16 arr, u16 psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 时钟使能

    TIM_Cmd(TIM2, DISABLE); // 先关闭定时器，确保配置时定时器停止
    TIM_ClearFlag(TIM2, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2 |
        TIM_FLAG_CC3 | TIM_FLAG_CC4); // 清除所有中断标志
    TIM_ITConfig(
        TIM2, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4,
        DISABLE); // 禁用所有中断

    // 中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 定时器TIM2初始化
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_GenerateEvent(
        TIM2, TIM_EventSource_Update); // 清除更新事件（避免立即产生更新事件）
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能中断
    TIM_Cmd(TIM2, ENABLE);                     // 使能定时器
}

// 通用定时器3初始化
// arr：自动重装值。
// psc：时钟预分频数
void TIM3_Init(u16 arr, u16 psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 时钟使能
    TIM_Cmd(TIM3, DISABLE); // 先关闭定时器，确保配置时定时器停止
    TIM_ClearFlag(TIM3, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2 |
        TIM_FLAG_CC3 | TIM_FLAG_CC4); // 清除所有中断标志
    TIM_ITConfig(
        TIM3, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4,
        DISABLE); // 禁用所有中断

    // 中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_GenerateEvent(
        TIM3, TIM_EventSource_Update); // 清除更新事件（避免立即产生更新事件）
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 使能中断
    TIM_Cmd(TIM3, ENABLE);                     // 使能定时器
}
