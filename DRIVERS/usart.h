#ifndef USART1_H_H
#define USART1_H_H
#include <stdint.h>
#include "stm32f10x.h"

#define MAX_RCV_LEN1 32
#define MAX_RCV_LEN2 32
#define MAX_RCV_LEN3 32

void USART1_Init(u32 baudRate);
void USART2_Init(u32 baudRate);
void USART3_Init(u32 baudRate);

void USART_Write(USART_TypeDef *USARTx, const u8 *buffa, u32 len);
void USART_Clear(USART_TypeDef *USARTx);

#endif
