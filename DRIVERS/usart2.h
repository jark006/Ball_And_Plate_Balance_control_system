#ifndef USART2_H_H
#define USART2_H_H
#include <stdint.h>
#include <stm32f10x.h>

#define MAX_RCV_LEN2 32

void USART2_Init(u32 baudRate);
void USART2_Clear(void);

#endif
