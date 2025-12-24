#ifndef USART1_H_H
#define USART1_H_H

#include <stdint.h>
#include <stm32f10x.h>

#define MAX_RCV_LEN1 256

void USART1_Config(u32 baudRate);
void usart_write(USART_TypeDef *USARTx, uint8_t *Data, uint8_t len);

#endif
