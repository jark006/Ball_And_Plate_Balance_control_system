#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "usart2.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "delay.h"
#include "sys.h"




unsigned char  usart2_rcv_buf[MAX_RCV_LEN2];
volatile unsigned int   usart2_rcv_len = 0;




/*
 *  @brief USART2��ʼ������
 */
void USART2_Config(u32 Baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

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
	USART_InitStructure.USART_BaudRate = Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);

	//Enable usart2 receive interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
*  @brief USART2���ڽ���״̬��ʼ��
*/
void USART2_Clear(void)
{
	memset(usart2_rcv_buf, 0, sizeof(usart2_rcv_buf));
	usart2_rcv_len = 0;
}

/*
*  @brief USART2����
*/
void USART2_Write(USART_TypeDef* USARTx, uint8_t *Data, uint8_t len)
{
	uint8_t i;
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	for(i = 0; i < len; i++)
	{
		USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}


/*
 *  @brief ����USART2�ѽ��յ����ݵ�buf������Ϊrcv_len
 */
void  USART2_GetRcvData(uint8_t *buf, uint32_t rcv_len)
{
	if(buf)
	{
		memcpy(buf, usart2_rcv_buf, rcv_len);
	}
	USART2_Clear();
}

