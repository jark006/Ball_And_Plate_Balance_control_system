#ifndef _PID_H_
#define _PID_H_

#include "stdint.h"
#include "stdio.h"
#include "stm32f10x.h"

typedef struct _POINT {
    uint16_t last;   // 上次坐标
    uint16_t now;    // 当前坐标
    uint16_t target; // 目标坐标

    uint16_t p1; // p1标识点
    uint16_t p2; // p2标识点
    uint16_t p3; // p3标识点
    uint16_t p4; // p4标识点
    uint16_t p5; // p5标识点
    uint16_t p6; // p6标识点
    uint16_t p7; // p7标识点
    uint16_t p8; // p8标识点
    uint16_t p9; // p9标识点

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
void PID_init(void);
int PID_realize(PID *pid);
#endif
