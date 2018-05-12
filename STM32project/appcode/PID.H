#ifndef _PID_H_
#define _PID_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "stdint.h"

typedef struct _POINT
{
	uint16_t last;    //�ϴ�����
	uint16_t now;     //��ǰ����
	uint16_t target;  //Ŀ������
	
	uint16_t p1;      //p1��ʶ��
	uint16_t p2;      //p2��ʶ��
	uint16_t p3;      //p3��ʶ��
	uint16_t p4;	    //p4��ʶ��
	uint16_t p5;      //p5��ʶ��
	uint16_t p6;      //p6��ʶ��
	uint16_t p7;      //p7��ʶ��
	uint16_t p8;	    //p8��ʶ��	
	uint16_t p9;      //p9��ʶ��

	int angle;   //����Ƕ�ƫֵ
	int anglewrite;
	int speed;
	int speedfb;
	
} Coordinate;       //����ṹ��


typedef struct _pid
{
	float SetValue;            //�����趨ֵ
	float ActualValue;        //����ʵ��ֵ
	float err;                //����ƫ��ֵ
	float err_last;            //������һ��ƫ��ֵ
	float Kp,Ki,Kd;            //������������֡�΢��ϵ��
	float voltage;            //�����ѹֵ������ִ�����ı�����
	float integral;            //�������ֵ
} PID;


extern PID pidx,pidy,pidxv,pidyv;
void PID_init(void);
float PID_realize(PID *pid);
float myPID_realize(PID *pid);
#endif