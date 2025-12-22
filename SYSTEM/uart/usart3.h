#ifndef usart3_H_H
#define usart3_H_H

#include <stm32f10x.h>
#include <stdint.h>



#define MAX_RCV_LEN3  256

extern void USART3_Config(u32 Baud);
extern void USART3_Write(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len);
extern void USART3_Clear(void);


extern unsigned char  usart3_rcv_buf[MAX_RCV_LEN3];
extern volatile unsigned int   usart3_rcv_len;

#endif

