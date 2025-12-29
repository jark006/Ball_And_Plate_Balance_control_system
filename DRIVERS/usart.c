
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"
#include "usart.h"

u8 usart1_rcv_buf[MAX_RCV_LEN1];
volatile u32 usart1_rcv_len = 0;

u8 usart2_rcv_buf[MAX_RCV_LEN2];
volatile u32 usart2_rcv_len = 0;

u8 usart3_rcv_buf[MAX_RCV_LEN3];
volatile u32 usart3_rcv_len = 0;

// USART1初始化
void USART1_Init(u32 baudRate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,
                           ENABLE);

    /* USART1 GPIO config */
    /* Set PA9 PA10 as UART1 */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

    // Enable usart1 receive interrupt
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*使用u-lib进行printf打印的实现，需要选择use u-lib*/
#ifdef __GNUC__
// With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
// set to 'Yes') calls __io_putchar()
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE {
    // Place your implementation of fputc here , e.g. write a character to the
    // USART
    USART_SendData(USART1, (u8)ch);
    // Loop until the end of transmission
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
    return ch;
}

/*
 *  @brief USART2初始化函数
 */
void USART2_Init(u32 baudRate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* config USART2 clock */
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA,
                           ENABLE);

    /* USART2 GPIO config */
    /* Set PA2 PA3 as UART1 */
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

    // Enable usart2 receive interrupt
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 *  @brief USART3初始化函数
 */
void USART3_Init(u32 baudRate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* config USART3 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    /* USART3 GPIO config */
    // USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // USART3_RX      PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* USART3 mode config */
    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);

    // Enable USART3 receive interrupt
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 *  @brief USART2串口接收状态初始化
 */
void USART_Clear(USART_TypeDef *USARTx){
    if(USARTx == USART1){
        memset(usart1_rcv_buf, 0, sizeof(usart1_rcv_buf));
        usart1_rcv_len = 0;
    }
    else if(USARTx == USART2){
        memset(usart2_rcv_buf, 0, sizeof(usart2_rcv_buf));
        usart2_rcv_len = 0;
    }
    else if(USARTx == USART3){
        memset(usart3_rcv_buf, 0, sizeof(usart3_rcv_buf));
        usart3_rcv_len = 0;
    }
}

/*
 *  @brief USART串口发送
 */
void USART_Write(USART_TypeDef *USARTx, const u8 *buff, u32 len) {
    while (len--) {
        USART_SendData(USARTx, *buff++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
}
