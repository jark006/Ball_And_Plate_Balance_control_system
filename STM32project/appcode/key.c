/************************************************************************************




*************************************************************************************/
#include "KEY.h"
#include "delay.h"



void Init_KEY(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	           //ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	 //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		             //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		             //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					                 //��ʼ��GPIOB

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE) ;        //�ر�JTAG������SW

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	 //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		               //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	            	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					                 //��ʼ��GPIOB

	K1 = K2 = K3 = K4 = K5 = K6 = 1;
}


u8 keynum = 0;
void key_scan(void)
{
	keynum = 0;
	
	K1 = K2 = K3 = K4 = K5 = K6 = 1;
	K1 = 0;
	if((K4==0)||(K5==0)||(K6==0))
	{
		delay_ms(DELAY);
		if(K4==0) keynum = 1;
		if(K5==0) keynum = 2;
		if(K6==0) keynum = 3;
		while((K4==0)||(K5==0)||(K6==0));
		delay_ms(DELAY);
		return;
	}
	
	K1 = K2 = K3 = K4 = K5 = K6 = 1;
	K2 = 0;
	if((K4==0)||(K5==0)||(K6==0))
	{
		delay_ms(DELAY);
		if(K4==0) keynum = 4;
		if(K5==0) keynum = 5;
		if(K6==0) keynum = 6;
		while((K4==0)||(K5==0)||(K6==0));
		delay_ms(DELAY);
		return;
	}
	
	K1 = K2 = K3 = K4 = K5 = K6 = 1;
	K3 = 0;
	if((K4==0)||(K5==0)||(K6==0))
	{
		delay_ms(DELAY);
		if(K4==0) keynum = 7;
		if(K5==0) keynum = 8;
		if(K6==0) keynum = 9;
		while((K4==0)||(K5==0)||(K6==0));
		delay_ms(DELAY);
		return;
	}
}
