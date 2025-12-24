/**
 ******************************************************************************
 * @file    GPIO/IOToggle/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 * peripherals interrupt service routine.
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
#include "pid.h"
#include "stdio.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"

extern Coordinate X, Y;

void NMI_Handler(void) {}

void HardFault_Handler(void) {
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}

void MemManage_Handler(void) {
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {
    }
}

void BusFault_Handler(void) {
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {
    }
}

void UsageFault_Handler(void) {
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {
    }
}

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

void PendSV_Handler(void) {}

void SysTick_Handler(void) {}

// TIM2中断
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除TIMx更新中断标志
        /**************************************************************************************/
        int xSpeed = (int)X.now - X.last;
        int ySpeed = (int)Y.now - Y.last;

        if (xSpeed > 15)
            xSpeed = 15;
        else if (xSpeed < -15)
            xSpeed = -15;
        if (ySpeed > 15)
            ySpeed = 15;
        else if (ySpeed < -15)
            ySpeed = -15;

        X.speed = xSpeed;
        Y.speed = ySpeed;
        /**************************************************************************************/
    }
}

// 定时器3中断服务程序
void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除TIMx更新中断标志
        /**************************************************************************************/

        /**************************************************************************************/
    }
}

extern uint8_t usart1_rcv_buf[MAX_RCV_LEN1];
extern volatile unsigned int usart1_rcv_len;

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        usart1_rcv_buf[usart1_rcv_len++] = USART1->DR; // 取出接收寄存器数据
        usart1_rcv_len &= 0XFF;                        // 防止越界
    }
}

extern unsigned char usart2_rcv_buf[MAX_RCV_LEN2];
extern volatile unsigned int usart2_rcv_len;

void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        usart2_rcv_buf[usart2_rcv_len++] = USART2->DR; // 取出接收寄存器数据
        usart2_rcv_len &= 0XFF;                        // 防止越界
    }
}

extern unsigned char usart3_rcv_buf[MAX_RCV_LEN3];
extern volatile unsigned int usart3_rcv_len;

void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        const uint8_t recvByte = USART3->DR;           // 取出接收寄存器数据
        usart3_rcv_buf[usart3_rcv_len++] = recvByte;
        usart3_rcv_len &= 0XFF;                        // 防止越界

        if ((recvByte == 0xfe) && (usart3_rcv_len >= 7)) {
            if ((usart3_rcv_buf[usart3_rcv_len - 7]) == 0xef &&
                usart3_rcv_buf[usart3_rcv_len - 6] == 0x0d) {
                X.last = X.now;
                Y.last = Y.now;
                X.now = (usart3_rcv_buf[usart3_rcv_len - 5] << 8) |usart3_rcv_buf[usart3_rcv_len - 4];
                Y.now = (usart3_rcv_buf[usart3_rcv_len - 3] << 8) | usart3_rcv_buf[usart3_rcv_len - 2];
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
