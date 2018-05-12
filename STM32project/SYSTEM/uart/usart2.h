#ifndef USART2_H_H
#define USART2_H_H

#include <stm32f10x.h>
#include <stdint.h>






#define MAX_RCV_LEN2  256

extern void USART2_Config(u32 Baud);
extern void USART2_Write(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len);
extern void USART2_Clear(void);


extern unsigned char  usart2_rcv_buf[MAX_RCV_LEN2];
extern volatile unsigned int   usart2_rcv_len;

#endif

