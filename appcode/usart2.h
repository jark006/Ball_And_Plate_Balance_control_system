#ifndef USART2_H_H
#define USART2_H_H
#include <stdint.h>
#include <stm32f10x.h>

#define MAX_RCV_LEN2 256
extern void USART2_Config(u32 baudRate);
extern void USART2_Clear(void);

#endif
