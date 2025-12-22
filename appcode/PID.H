#ifndef _PID_H_
#define _PID_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "stdint.h"

typedef struct _POINT
{
	uint16_t last;    //上次坐标
	uint16_t now;     //当前坐标
	uint16_t target;  //目标坐标
	
	uint16_t p1;      //p1标识点
	uint16_t p2;      //p2标识点
	uint16_t p3;      //p3标识点
	uint16_t p4;	    //p4标识点
	uint16_t p5;      //p5标识点
	uint16_t p6;      //p6标识点
	uint16_t p7;      //p7标识点
	uint16_t p8;	    //p8标识点	
	uint16_t p9;      //p9标识点

	int angle;   //舵机角度偏值
	int anglewrite;
	int speed;
	int speedfb;
	
} Coordinate;       //坐标结构体


typedef struct _pid
{
	float SetValue;            //定义设定值
	float ActualValue;        //定义实际值
	float err;                //定义偏差值
	float err_last;            //定义上一个偏差值
	float Kp,Ki,Kd;            //定义比例、积分、微分系数
	float voltage;            //定义电压值（控制执行器的变量）
	float integral;            //定义积分值
} PID;


extern PID pidx,pidy,pidxv,pidyv;
void PID_init(void);
float PID_realize(PID *pid);
float myPID_realize(PID *pid);
#endif
