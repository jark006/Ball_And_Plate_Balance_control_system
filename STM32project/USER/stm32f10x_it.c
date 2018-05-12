/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "stdio.h"
#include "PID.h"
#include "usart1.h" 
#include "usart2.h" 
#include "usart3.h" 


extern Coordinate X,Y;



void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
//void SVC_Handler(void)
//{
//}
// 
//void DebugMon_Handler(void)
//{
//}
// 
//void PendSV_Handler(void)
//{
//}
// 
//void SysTick_Handler(void)
//{
//}

u16 count = 0;
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	int xtmp,ytmp;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־
		/**************************************************************************************/
		
		//count ++;
		//printf("TIM2:%d\r\n",count);		
		//OLED_ShowNum(0,2,count,5,16);
		xtmp = X.now - X.last;
		ytmp = Y.now - Y.last;
//		
//		if(xtmp < 15 && xtmp > (-15))  
//			pidxv.ActualValue = X.speed = xtmp;
//		if(ytmp < 15 && ytmp > (-15))  
//			pidyv.ActualValue = Y.speed = ytmp;
				
		if(xtmp < 15 && xtmp > (-15))  
			X.speed = xtmp;
		if(ytmp < 15 && ytmp > (-15))  
			Y.speed = ytmp;
		
		
		/**************************************************************************************/
	}
	
	
}



//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־
		/**************************************************************************************/
		

		
		
		
		
		/**************************************************************************************/
	}
}



void USART1_IRQHandler(void)
{
	uint8_t Res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   //�жϽ����ж�
	{
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//ȡ�����ռĴ�������
		usart1_rcv_buf[usart1_rcv_len++] = Res;
	}
}

void USART2_IRQHandler(void)
{
	uint8_t Res;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   //�жϽ����ж�
	{
		Res = USART_ReceiveData(USART2);//(USART2->DR);	//ȡ�����ռĴ�������
		usart2_rcv_buf[usart2_rcv_len++] = Res;
	}
}

void USART3_IRQHandler(void)
{
	uint8_t Res;
	//uint16_t X_tmp,Y_tmp;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)   //�жϽ����ж�
	{
		Res = USART_ReceiveData(USART3);//(USART2->DR);	//ȡ�����ռĴ�������
		usart3_rcv_buf[usart3_rcv_len++] = Res;
		
		if((Res == 0xfe) && (usart3_rcv_len>=7))
		{
			if((usart3_rcv_buf[usart3_rcv_len-7])==0xef && usart3_rcv_buf[usart3_rcv_len-6] == 0x0d)
			{
				X.last = X.now;
				Y.last = Y.now;
				X.now = usart3_rcv_buf[usart3_rcv_len-4];
				Y.now = usart3_rcv_buf[usart3_rcv_len-2];
				usart3_rcv_len = 0;
			}
		}

		
	}
}












/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
