/***********************************************************************************************************
 *  2017全国大学生电子设计竞赛广东赛区
 *  题目：滚球控制系统 （ B 题 ）
 *  作者：陈思杰
 ***********************************************************************************************************/

#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "oled.h"
#include "pid.h"
#include "servo.h"
#include "timer.h"
#include "usart.h"

typedef struct {
    void (*fun)(void);
    const char* name;
} FunctionStruct;

void ShowBall(void);
void SetPoint(void);

void Base1(void);
void Base2(void);
void Base3(void);
void Base4(void);
void More1(void);
void More2(void);
void More3(void);
void More4(void);

void More2Test();

// 初始化所有坐标
Coordinate X = { 0, 0, 0, { 0, 22, 66, 110, 22, 66, 110, 22, 66, 110 }, 0, 0, 0, 0 };
Coordinate Y = { 0, 0, 0, { 0, 17, 17, 17, 62, 62, 62, 105, 105, 105 }, 0, 0, 0, 0 };

/***********************************************************************************************************
主程序
***********************************************************************************************************/
int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 2位抢占，2位响应
    Delay_Init();         // 延时初始化
    USART1_Init(115200);  // 串口1 打印调试
    USART2_Init(115200);  // 串口2 控制舵机
    USART3_Init(115200);  // 串口3 接收OpenMV的小球位置
    TIM2_Init(300, 7200); // 定时30ms
    KEY_Init();
    OLED_Init();

    const FunctionStruct FunctionTable[] = {
        {ShowBall, "ShowBall"}, {SetPoint, "SetPoint"}, {Base1, "Base 1  "},
        {Base2, "Base 2  "},    {Base3, "Base 3  "},    {Base4, "Base 4  "},
        {More1, "More 1  "},    {More2, "More 2  "},    {More3, "More 3  "},
        {More4, "More 4  "},
    };
    u8 funcIdx = 0;
    u8 isNeedRefresh = 1;

    OLED_Clear();
    PID_Init();
    ServoResetPosition();

    while (1) {
        key_scan();
        if ((UP || LEFT) && funcIdx > 0) {
            funcIdx--;
            isNeedRefresh = 1;
        }
        if ((DOWN || RIGHT) && funcIdx < (sizeof(FunctionTable) / sizeof(FunctionStruct) - 1)) {
            funcIdx++;
            isNeedRefresh = 1;
        }
        if (CONF) {
            FunctionTable[funcIdx].fun();
            isNeedRefresh = 1; // 从以上功能退出时，要刷新显示
            OLED_Clear();
        }
        if (isNeedRefresh) {
            isNeedRefresh = 0;
            OLED_ShowString(10, 0, "Select Func", 16);
            OLED_ShowString(10, 4, FunctionTable[funcIdx].name, 16);
        }
    }
}

// 限制舵机转角在合理范围内
static inline void limitServoAngle() {
    const int ANGLE_MAX = 700; // 舵机最大转角
    const int ANGLE_MIN = 300; // 舵机最小转角

    if (X.anglewrite > ANGLE_MAX)
        X.anglewrite = ANGLE_MAX;
    else if (X.anglewrite < ANGLE_MIN)
        X.anglewrite = ANGLE_MIN;

    if (Y.anglewrite > ANGLE_MAX)
        Y.anglewrite = ANGLE_MAX;
    else if (Y.anglewrite < ANGLE_MIN)
        Y.anglewrite = ANGLE_MIN;
}

/***********************************************************************************************************
基础项目：1
***********************************************************************************************************/
void Base1() {
    PID_Init();
    X.target = X.p[2];
    Y.target = Y.p[2];
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    X.speed = 0;
    Y.speed = 0;

    OLED_Clear();
    OLED_ShowString(0, 0, "Base 1", 16);
    OLED_ShowString(0, 2, "stay on 2 in 5s.", 16);

    while (1) {
        key_scan();
        if (CONF) {
            while (1) {
                key_scan();
                pidx.ActualValue = X.now;
                pidy.ActualValue = Y.now;
                X.angle = PID_realize(&pidx);
                Y.angle = PID_realize(&pidy);
                X.anglewrite = 500 + (X.speed * 60) - X.angle;
                Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
                limitServoAngle();
                ServoMove(ID1, X.anglewrite);
                ServoMove(ID2, Y.anglewrite);
                if (EXIT) {
                    OLED_Clear();
                    break;
                }
            }
        }
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    ServoResetPosition();
}

/***********************************************************************************************************
基础项目：2
***********************************************************************************************************/
void Base2() {
    int Xsp, Ysp;

    PID_Init();
    X.target = X.p[5];
    Y.target = Y.p[5];
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    pidx.Ki = pidy.Ki = 0.0035;
    X.speed = 0;
    Y.speed = 0;

    OLED_Clear();
    OLED_ShowString(0, 0, "Base 2", 16);
    OLED_ShowString(0, 2, "Move from 1 to 5", 16);

    while (1) {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);

        if (abs(X.speed) < 3)
            Xsp = X.speed * 60;
        else
            Xsp = X.speed * 85;
        if (abs(Y.speed) < 3)
            Ysp = Y.speed * 60;
        else
            Ysp = Y.speed * 85;
        X.anglewrite = 500 + Xsp - X.angle;
        Y.anglewrite = 500 + Ysp - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    ServoResetPosition();
}

/***********************************************************************************************************
基础项目：3
***********************************************************************************************************/
void Base3() {
    u32 time_count = 0;

    PID_Init();
    X.now = X.target = X.p[4];
    Y.now = Y.target = Y.p[4];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;

    OLED_Clear();
    OLED_ShowString(0, 0, "Base 3", 16);
    OLED_ShowString(0, 2, "Move from 1 to 4,stay 2s", 16);
    OLED_ShowString(0, 4, "then 4 to 5", 16);

    OLED_ShowString(30, 6, "Part 1", 16);
    while (1) {
        time_count++;
        if (time_count == 1000)
            pidx.integral = 0;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 95) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 95) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 5000)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 2", 16);
    PID_Init();
    X.now = X.target = X.p[5];
    Y.now = Y.target = Y.p[5];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;

    while (1) {
        time_count++;
        if (time_count == 700)
            pidx.integral = 0;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 90) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 90) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    ServoResetPosition();
}

/***********************************************************************************************************
基础项目：4
***********************************************************************************************************/
void Base4() {
    u32 time_count = 0;

    PID_Init();
    X.now = X.target = X.p[5];
    Y.now = Y.target = Y.p[5];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;

    OLED_Clear();
    OLED_ShowString(0, 0, "Base 4", 16);
    OLED_ShowString(0, 2, "Move from 1 to 9,stay 2s", 16);

    OLED_ShowString(30, 6, "Part 1", 16);
    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 1500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 2", 16);
    PID_Init();
    X.now = X.target = 75;
    Y.now = Y.target = 75;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 1500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 3", 16);
    PID_Init();
    X.now = X.target = 85;
    Y.now = Y.target = 85;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 75) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 70) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 1500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 4", 16);
    PID_Init();
    X.now = X.target = X.p[9];
    Y.now = Y.target = Y.p[9];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while (1) {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 80) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 80) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    ServoResetPosition();
}

/***********************************************************************************************************
发挥项目：1
***********************************************************************************************************/
void More1() {
    u32 time_count = 0;
    OLED_Clear();
    OLED_ShowString(0, 0, "More 1", 16);
    OLED_ShowString(0, 2, "Move by points", 16);
    OLED_ShowString(0, 4, "1 > 2 > 6 > 9 ", 16);

    PID_Init();
    X.now = X.target = X.p[2];
    Y.now = Y.target = Y.p[2] + 5;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;

    OLED_ShowString(30, 6, "Part 1", 16);
    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 2", 16);
    PID_Init();
    X.now = X.target = (X.p[2] + X.p[6]) / 2;
    Y.now = Y.target = (Y.p[2] + Y.p[6]) / 2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;

    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 3", 16);
    PID_Init();
    X.now = X.target = X.p[6] - 5;
    Y.now = Y.target = Y.p[6] + 5;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;

    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 3000)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 4", 16);
    PID_Init();
    X.now = X.target = (X.p[9] + X.p[6]) / 2;
    Y.now = Y.target = (Y.p[9] + Y.p[6]) / 2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;

    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 5", 16);
    PID_Init();
    X.now = X.target = X.p[9];
    Y.now = Y.target = Y.p[9];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;

    while (1) {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 70) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 70) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    ServoResetPosition();
}

/***********************************************************************************************************
发挥项目：2
***********************************************************************************************************/
void More2() {
    u32 time_count = 0;

    OLED_Clear();
    OLED_ShowString(0, 0, "More 2", 16);
    OLED_ShowString(0, 2, "Move by points", 16);
    OLED_ShowString(0, 4, "A > B > C > D ", 16);
    OLED_ShowString(30, 6, "Part 2", 16);

    PID_Init();
    X.now = X.target = (X.p[2] + X.p[1]) / 2;
    Y.now = Y.target = (Y.p[2] + Y.p[1]) / 2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;

    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 1", 16);
    PID_Init();
    X.now = X.target = X.p[2];
    Y.now = Y.target = Y.p[2];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;

    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 2", 16);
    PID_Init();
    X.now = X.target = (X.p[2] + X.p[6]) / 2;
    Y.now = Y.target = (Y.p[2] + Y.p[6]) / 2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;

    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 3", 16);
    PID_Init();
    X.now = X.target = X.p[6];
    Y.now = Y.target = Y.p[6];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;

    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 80) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 80) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 3000)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 4", 16);
    PID_Init();
    X.now = X.target = (X.p[9] + X.p[6]) / 2;
    Y.now = Y.target = (Y.p[9] + Y.p[6]) / 2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;

    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 80) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 80) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }

    OLED_ShowString(30, 6, "Part 5", 16);
    PID_Init();
    X.now = X.target = X.p[9];
    Y.now = Y.target = Y.p[9];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;

    while (1) {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 75) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 75) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    ServoResetPosition();
}

/***********************************************************************************************************
发挥项目：3
***********************************************************************************************************/
void More3() {
    int time_count = 0;
    int cc = 3;
    OLED_Clear();
    OLED_ShowString(0, 0, "More 3", 16);
    OLED_ShowString(0, 2, "Move soround 5", 16);
    OLED_ShowString(30, 6, "Part 2", 16);

    while (cc--) {
        PID_Init();
        X.now = X.target = X.p[5] + 8;
        Y.now = Y.target = Y.p[5];
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        pidx.SetValue = X.target;
        pidy.SetValue = Y.target;
        time_count = 0;

        while (1) {
            time_count++;
            key_scan();
            pidx.ActualValue = X.now;
            pidy.ActualValue = Y.now;
            X.angle = PID_realize(&pidx);
            Y.angle = PID_realize(&pidy);
            X.anglewrite = 500 + (X.speed * 60) - X.angle;
            Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
            limitServoAngle();
            ServoMove(ID1, X.anglewrite);
            ServoMove(ID2, Y.anglewrite);
            if (time_count > 2500)
                break;
            if (EXIT) {
                OLED_Clear();
                break;
            }
        }

        OLED_ShowString(30, 6, "Part 1", 16);
        PID_Init();
        X.now = X.target = X.p[5] + 8;
        Y.now = Y.target = Y.p[5];
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        pidx.SetValue = X.target;
        pidy.SetValue = Y.target;
        while (1) {
            time_count++;
            key_scan();
            pidx.ActualValue = X.now;
            pidy.ActualValue = Y.now;
            X.angle = PID_realize(&pidx);
            Y.angle = PID_realize(&pidy);
            X.anglewrite = 500 + (X.speed * 60) - X.angle;
            Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
            limitServoAngle();
            ServoMove(ID1, X.anglewrite);
            ServoMove(ID2, Y.anglewrite);
            if (time_count > 2500)
                break;
            if (EXIT) {
                OLED_Clear();
                break;
            }
        }

        OLED_ShowString(30, 6, "Part 2", 16);
        PID_Init();
        X.now = X.target = X.p[5];
        Y.now = Y.target = Y.p[5] - 8;
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        pidx.SetValue = X.target;
        pidy.SetValue = Y.target;
        time_count = 0;
        while (1) {
            time_count++;
            key_scan();
            pidx.ActualValue = X.now;
            pidy.ActualValue = Y.now;
            X.angle = PID_realize(&pidx);
            Y.angle = PID_realize(&pidy);
            X.anglewrite = 500 + (X.speed * 60) - X.angle;
            Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
            limitServoAngle();
            ServoMove(ID1, X.anglewrite);
            ServoMove(ID2, Y.anglewrite);
            if (time_count > 2500)
                break;
            if (EXIT) {
                OLED_Clear();
                break;
            }
        }
        OLED_ShowString(30, 6, "Part 3", 16);
        PID_Init();
        X.now = X.target = X.p[5] - 8;
        Y.now = Y.target = Y.p[5];
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        pidx.SetValue = X.target;
        pidy.SetValue = Y.target;
        time_count = 0;
        while (1) {
            time_count++;
            key_scan();
            pidx.ActualValue = X.now;
            pidy.ActualValue = Y.now;
            X.angle = PID_realize(&pidx);
            Y.angle = PID_realize(&pidy);
            X.anglewrite = 500 + (X.speed * 80) - X.angle;
            Y.anglewrite = 500 + (Y.speed * 80) - Y.angle;
            limitServoAngle();
            ServoMove(ID1, X.anglewrite);
            ServoMove(ID2, Y.anglewrite);
            if (time_count > 3000)
                break;
            if (EXIT) {
                OLED_Clear();
                break;
            }
        }
    }

    OLED_ShowString(30, 6, "Part 5", 16);
    PID_Init();
    X.now = X.target = X.p[9];
    Y.now = Y.target = Y.p[9];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;

    while (1) {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 75) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 75) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    ServoResetPosition();
}

/***********************************************************************************************************
发挥项目：2  测试
***********************************************************************************************************/
void More2Test() {
    u32 time_count = 0;
    OLED_Clear();
    OLED_ShowString(0, 0, "More 1", 16);
    OLED_ShowString(0, 2, "Move by points", 16);
    OLED_ShowString(0, 2, "A > B > C > D", 16);
    PID_Init();
    X.now = X.target = X.p[2];
    Y.now = Y.target = Y.p[2];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    OLED_ShowString(30, 6, "Part 1", 16);
    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30, 6, "Part 2", 16);
    PID_Init();
    X.now = X.target = (X.p[2] + X.p[6]) / 2;
    Y.now = Y.target = (Y.p[2] + Y.p[6]) / 2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 60) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 60) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30, 6, "Part 3", 16);
    PID_Init();
    X.now = X.target = X.p[6];
    Y.now = Y.target = Y.p[6];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 80) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 80) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 3000)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30, 6, "Part 4", 16);
    PID_Init();
    X.now = X.target = (X.p[9] + X.p[6]) / 2;
    Y.now = Y.target = (Y.p[9] + Y.p[6]) / 2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while (1) {
        time_count++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 80) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 80) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (time_count > 2500)
            break;
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    //*****************************************************************************
    OLED_ShowString(30, 6, "Part 5", 16);
    PID_Init();
    X.now = X.target = X.p[9];
    Y.now = Y.target = Y.p[9];
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while (1) {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = PID_realize(&pidx);
        Y.angle = PID_realize(&pidy);
        X.anglewrite = 500 + (X.speed * 75) - X.angle;
        Y.anglewrite = 500 + (Y.speed * 75) - Y.angle;
        limitServoAngle();
        ServoMove(ID1, X.anglewrite);
        ServoMove(ID2, Y.anglewrite);
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
    ServoResetPosition();
}

/***********************************************************************************************************
发挥项目：4
***********************************************************************************************************/
void More4() {
    OLED_Clear();
    OLED_ShowString(0, 0, "More 4", 16);
    OLED_ShowString(0, 2, "stay on 2 in 5s.", 16);
    PID_Init();
    X.now = X.target = 63;
    Y.now = Y.target = 60;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while (1) {
        key_scan();
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
}

// 实时显示小球坐标，按F1键记录当前位置为目标位置
void ShowBall() {
    OLED_Clear();
    OLED_ShowString(0, 0, "====ShowBall====", 16);
    OLED_ShowString(0, 2, " X:", 16);
    OLED_ShowString(0, 4, " Y:", 16);
    while (1) {
        key_scan();
        if (F1) {
            X.target = X.now;
            Y.target = Y.now;
            pidx.SetValue = X.now;
            pidy.SetValue = Y.now;
            OLED_ShowNum(80, 2, X.target, 5, 16);
            OLED_ShowNum(80, 4, Y.target, 5, 16);
        }
        OLED_ShowNum(32, 2, X.now, 5, 16);
        OLED_ShowNum(32, 4, Y.now, 5, 16);
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
}

// 设置各点坐标，把黑色小球放到对应点上，按CONF键记录当前坐标值
void SetPoint() {
    char pointStr[4] = "Px";
    u8 pointIdx = 5; // 1~9 对应 p1-p9
    OLED_Clear();
    OLED_ShowString(0, 0, "====SetPoint====", 16);
    OLED_ShowString(0, 2, "Set:     X   Y", 16);
    OLED_ShowString(0, 4, "Value", 16);
    OLED_ShowString(0, 6, "Ball", 16);
    while (1) {
        key_scan();
        OLED_ShowNum(40, 6, X.now, 5, 16);
        OLED_ShowNum(88, 6, Y.now, 5, 16);
        if ((UP || LEFT) && pointIdx > 1)
            pointIdx--;
        if ((DOWN || RIGHT) && pointIdx < 9)
            pointIdx++;

        pointStr[1] = pointIdx + '0';
        OLED_ShowString(32, 2, pointStr, 16);
        OLED_ShowNum(40, 4, X.p[pointIdx], 5, 16);
        OLED_ShowNum(88, 4, Y.p[pointIdx], 5, 16);

        if (CONF) {
            X.p[pointIdx] = X.now;
            Y.p[pointIdx] = Y.now;
        }
        if (EXIT) {
            OLED_Clear();
            break;
        }
    }
}
