#ifndef USART1_H_H
#define USART1_H_H

#include <stm32f10x.h>
#include <stdint.h>


#define MAX_RCV_LEN1  256


extern void USART1_Config(u32 Baud);
extern void usart1_write(USART_TypeDef* USARTx, uint8_t *Data, uint8_t len);

extern uint8_t usart1_rcv_buf[MAX_RCV_LEN1];
extern volatile unsigned int  usart1_rcv_len;



#endif

