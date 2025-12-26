#ifndef usart3_H_H
#define usart3_H_H
#include <stdint.h>
#include <stm32f10x.h>

#define MAX_RCV_LEN3 256

void USART3_Init(u32 baudRate);
void USART3_Clear(void);

#endif
