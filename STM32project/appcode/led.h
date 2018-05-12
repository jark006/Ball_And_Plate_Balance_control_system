#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "sys.h"


#define LED  PAout(2)

#define LED_OFF GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define LED_ON GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define LED_REV GPIO_WriteBit(GPIOA, GPIO_Pin_2,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2))))

void LED_Init(void);

#endif
