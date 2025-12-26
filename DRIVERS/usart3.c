#include "usart3.h"
#include "delay.h"
#include "misc.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"
#include "string.h"
#include "sys.h"

unsigned char usart3_rcv_buf[MAX_RCV_LEN3];
volatile unsigned int usart3_rcv_len = 0;

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
 *  @brief USART3串口接收状态初始化
 */
void USART3_Clear(void) {
    memset(usart3_rcv_buf, 0, sizeof(usart3_rcv_buf));
    usart3_rcv_len = 0;
}

/*
 *  @brief 返回USART3已接收的数据到buf，长度为rcv_len
 */
void USART3_GetRcvData(uint8_t *buf, uint32_t rcv_len) {
    if (buf) {
        memcpy(buf, usart3_rcv_buf, rcv_len);
    }
    USART3_Clear();
}
