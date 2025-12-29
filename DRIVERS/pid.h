#ifndef _PID_H_
#define _PID_H_

#include "stdint.h"
#include "stdio.h"
#include "stm32f10x.h"

typedef struct _POINT {
    uint16_t last;   // 上次坐标
    uint16_t now;    // 当前坐标
    uint16_t target; // 目标坐标

    uint16_t p[10]; // idx:0 首位预留， idx: 1~9 对应 p1~p9标识点

    int angle; // 舵机角度偏值
    int anglewrite;
    int speed;
    int speedfb;

} Coordinate; // 坐标结构体

typedef struct _pid {
    float SetValue;    // 目标值
    float ActualValue; // 当前值
    float err;         // 偏差值
    float err_last;    // 上次偏差值
    float integral;    // 积分值
    float Kp, Ki, Kd;  // 比例、积分、微分系数
} PID;

extern PID pidx, pidy;
void PID_Init(void);
int PID_realize(PID *pid);
#endif
