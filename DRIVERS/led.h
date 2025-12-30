#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "sys.h"

#define LED_OFF do{ GPIOA->BSRR = GPIO_Pin_2; }while(0)
#define LED_ON  do{ GPIOA->BRR = GPIO_Pin_2; }while(0)
#define LED_TOGGLE do{ GPIOA->ODR ^= GPIO_Pin_2; }while(0)

void LED_Init(void);

#endif
