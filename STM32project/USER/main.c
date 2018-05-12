/***********************************************************************************************************
*
*  2017全国大学生电子设计竞赛广东赛区
*
*  题目：滚球控制系统 （ B 题 ）
*
*  作者：陈思杰
*
***********************************************************************************************************/
#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "oled.h"
#include "LobotSerialServo.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>
#include "key.h"
#include "PID.h"
#include "math.h"
#define X_angle_up   700  //舵机转角上下限
#define X_angle_down 300
void ShowBall(void);
void SetPoint(void);
void SetPID(void);
void Pidtest(void);
void Funlist(void);
void Fun1(void);
void Fun2(void);
void Fun3(void);
void Fun4(void);
void Fun5(void);
void Fun6(void);
void Fun7(void);
void Fun8(void);
//初始化所有坐标
Coordinate
X = {0,0,0,22,66,110,  22,66,110,  22,66,110, 0,0,0,0},
Y = {0,0,0,17,17,17,  62,62,62,  105,105,105, 0,0,0,0};
uint16_t i = 0;
uint8_t mainItem = 0;
/***********************************************************************************************************
主程序
***********************************************************************************************************/
int main(void)
{
    SystemInit();   //系统时钟等初始化
    delay_init();		//延时初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    USART1_Config(115200);//串口初始化为115200
    USART2_Config(115200);//串口初始化为115200
    USART3_Config(115200);//串口初始化为115200
    Init_KEY();
    PID_init();
    OLED_Init();
    OLED_Clear();
    TIM2_Init( 600,3600); //定时30ms
    Funlist();
    X.now = X.target =  63;
    Y.now = Y.target =  60;
    //ShowBall();
    printf("START\r\n");
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    pidxv.SetValue = 0;
    pidyv.SetValue = 0;
    OLED_ShowString(10, 0,(u8*)"ShowBall",16);
    OLED_ShowString(10, 2,(u8*)"SetPoint",16);
    OLED_ShowString(10, 4,(u8*)"SetPID",  16);
    OLED_ShowString(10, 6,(u8*)"Pidtest", 16);
    OLED_ShowString(0, mainItem * 2 ,(u8*)">", 16);
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
    while(1)
    {
        key_scan();
        if(UP && mainItem > 0)
        {
            mainItem --;
            for(i=0; i<4; i++)
                OLED_ShowString(0, i * 2 ,(u8*)" ", 16);
            OLED_ShowString(0, mainItem * 2 ,(u8*)">", 16);
        }
        if(DOWN && mainItem < 3)
        {
            mainItem ++;
            for(i=0; i<4; i++)
                OLED_ShowString(0, i * 2 ,(u8*)" ", 16);
            OLED_ShowString(0, mainItem * 2 ,(u8*)">", 16);
        }
        if(CONF)
        {
            switch(mainItem)
            {
            case 0:
                ShowBall();
                break;
            case 1:
                SetPoint();
                break;
            case 2:
                SetPID();
                break;
            case 3:
                Pidtest();
                break;
            default:
                break;
            }
            OLED_ShowString(10, 0,(u8*)"ShowBall",16);
            OLED_ShowString(10, 2,(u8*)"SetPoint",16);
            OLED_ShowString(10, 4,(u8*)"SetPID",  16);
            OLED_ShowString(10, 6,(u8*)"Pidtest", 16);
            OLED_ShowString(0, mainItem * 2 ,(u8*)">", 16);
        }
    }
}
static u8 FunctionList[][20] =
{
    "ShowBall",
    "SetPoint",
    "Item 1  ",
    "Item 2  ",
    "Item 3  ",
    "Item 4  ",
    "More 1  ",
    "More 2  ",
    "More 3  ",
    "More 4  ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
};
/***********************************************************************************************************
***********************************************************************************************************/
void Funlist()
{
    u8 item = 0;
    u8 ref_flag = 1;
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
    OLED_Clear();
    OLED_ShowString(10, 0,(u8*)"Selet item", 16);
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
    while(1)
    {
        key_scan();
        if((UP||LEFT) && item > 0)
        {
            item --;
            ref_flag = 1;
        }
        if((DOWN||RIGHT) && item < 10)
        {
            item ++;
            ref_flag = 1;
        }
        if(ref_flag==1)
        {
            ref_flag = 0;
            OLED_ShowString(10, 4,FunctionList[item], 16);
        }
        if(CONF)
        {
            switch(item)
            {
            case 0:
                ShowBall();
                break;
            case 1:
                SetPoint();
                break;
            case 2:
                Fun1();
                break;
            case 3:
                Fun2();
                break;
            case 4:
                Fun3();
                break;
            case 5:
                Fun4();
                break;
            case 6:
                Fun5();
                break;
            case 7:
                Fun6();
                break;
            case 8:
                Fun7();
                break;
            case 9:
                Fun8();
                break;
            default:
                break;
            }
            ref_flag = 1;
            OLED_ShowString(10, 0,(u8*)"Selet item", 16);
        }
    }
}
/***********************************************************************************************************
基础项目：1
***********************************************************************************************************/
void Fun1()
{
    OLED_Clear();
    OLED_ShowString(0,0,(u8*)"Item 1",16);
    OLED_ShowString(0,2,(u8*)"stay on 2 in 5s.",16);
    PID_init();
    X.target =  X.p2;
    Y.target =  Y.p2;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    X.speed = 0;
    Y.speed = 0;
    while(1)
    {
        key_scan();
        if(CONF)
        {
            while(1)
            {
                key_scan();
                pidx.ActualValue = X.now;
                pidy.ActualValue = Y.now;
                X.angle = (int)PID_realize(&pidx);
                Y.angle = (int)PID_realize(&pidy);
                X.anglewrite = 500  + (X.speed * 60) - X.angle;
                Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
                if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
                if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
                if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
                if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
                SetServo(ID1,X.anglewrite);
                SetServo(ID2,Y.anglewrite);
                if(EXIT)
                {
                    OLED_Clear();
                    break;
                }
            }
        }
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
}
/***********************************************************************************************************
基础项目：2
***********************************************************************************************************/
void Fun2()
{
    int Xsp,Ysp;
    OLED_Clear();
    OLED_ShowString(0,0,(u8*)"Item 2",16);
    OLED_ShowString(0,2,(u8*)"Move from 1 to 5",16);
    PID_init();
    X.target =  X.p5;
    Y.target =  Y.p5;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    pidx.Ki = pidy.Ki = 0.0035;
    X.speed = 0;
    Y.speed = 0;
    while(1)
    {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        //printf("Xspeed:%d Yspeed:%d\r\n",X.speed,Y.speed);
        if(abs(X.speed)<3)
            Xsp = X.speed * 60;
        else
            Xsp = X.speed * 85;
        if(abs(Y.speed)<3)
            Ysp = Y.speed * 60;
        else
            Ysp = Y.speed * 85;
        X.anglewrite = 500  + Xsp - X.angle;
        Y.anglewrite = 500  + Ysp - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
}
/***********************************************************************************************************
基础项目：3
***********************************************************************************************************/
void Fun3()
{
    uint32_t time_count = 0;
    OLED_Clear();
    OLED_ShowString(0,0,(u8*)"Item 3",16);
    OLED_ShowString(0,2,(u8*)"Move from 1 to 4,stay 2s",16);
    OLED_ShowString(0,4,(u8*)"then 4 to 5",16);
    PID_init();
    X.now = X.target =  X.p4;
    Y.now = Y.target =  Y.p4;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    OLED_ShowString(30,6,(u8*)"Part 1",16);
    while(1)
    {
        time_count ++;
        if(time_count == 1000) pidx.integral = 0;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 95) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 95) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 5000)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 2",16);
    PID_init();
    X.now = X.target =  X.p5;
    Y.now = Y.target =  Y.p5;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        if(time_count == 700) pidx.integral = 0;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 90) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 90) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
}
/***********************************************************************************************************
基础项目：4
***********************************************************************************************************/
void Fun4()
{
    uint32_t time_count = 0;
    OLED_Clear();
    OLED_ShowString(0,0,(u8*)"Item 4",16);
    OLED_ShowString(0,2,(u8*)"Move from 1 to 9,stay 2s",16);
    //OLED_ShowString(0,4,(u8*)"then 4 to 5",16);
    PID_init();
    X.now = X.target =  X.p5;
    Y.now = Y.target =  Y.p5;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    OLED_ShowString(30,6,(u8*)"Part 1",16);
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 1500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 2",16);
    PID_init();
    X.now = X.target =  75;
    Y.now = Y.target =  75;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 1500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 3",16);
    PID_init();
    X.now = X.target =  85;
    Y.now = Y.target =  85;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 75) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 70) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 1500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 4",16);
    PID_init();
    X.now = X.target =  X.p9;
    Y.now = Y.target =  Y.p9;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while(1)
    {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 80) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 80) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
}
/***********************************************************************************************************
发挥项目：1
***********************************************************************************************************/
void Fun5()
{
    uint32_t time_count = 0;
    OLED_Clear();
    OLED_ShowString(0,0,(u8*)"More 1",16);
    OLED_ShowString(0,2,(u8*)"Move by points",16);
    OLED_ShowString(0,4,(u8*)"1 > 2 > 6 > 9 ",16);
    //OLED_ShowString(0,4,(u8*)"then 4 to 5",16);
    PID_init();
    X.now = X.target =  X.p2;
    Y.now = Y.target =  Y.p2 + 5;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    OLED_ShowString(30,6,(u8*)"Part 1",16);
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 2",16);
    PID_init();
    X.now = X.target =  (X.p2+X.p6)/2;
    Y.now = Y.target =  (Y.p2+Y.p6)/2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 3",16);
    PID_init();
    X.now = X.target = X.p6 - 5;
    Y.now = Y.target = Y.p6 + 5;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 3000)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 4",16);
    PID_init();
    X.now = X.target =  (X.p9+X.p6)/2;
    Y.now = Y.target =  (Y.p9+Y.p6)/2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 5",16);
    PID_init();
    X.now = X.target =  X.p9;
    Y.now = Y.target =  Y.p9;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while(1)
    {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 70) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 70) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
}
/***********************************************************************************************************
发挥项目：2
***********************************************************************************************************/
void Fun6()
{
    uint32_t time_count = 0;
    OLED_Clear();
    OLED_ShowString(0,0,(u8*)"More 2",16);
    OLED_ShowString(0,2,(u8*)"Move by points",16);
    OLED_ShowString(0,4,(u8*)"A > B > C > D ",16);
    OLED_ShowString(30,6,(u8*)"Part 2",16);
    PID_init();
    X.now = X.target =  (X.p2+X.p1)/2;
    Y.now = Y.target =  (Y.p2+Y.p1)/2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    PID_init();
    X.now = X.target =  X.p2;
    Y.now = Y.target =  Y.p2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    OLED_ShowString(30,6,(u8*)"Part 1",16);
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 2",16);
    PID_init();
    X.now = X.target =  (X.p2+X.p6)/2;
    Y.now = Y.target =  (Y.p2+Y.p6)/2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 3",16);
    PID_init();
    X.now = X.target = X.p6;
    Y.now = Y.target = Y.p6;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 80) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 80) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 3000)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 4",16);
    PID_init();
    X.now = X.target =  (X.p9+X.p6)/2;
    Y.now = Y.target =  (Y.p9+Y.p6)/2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 80) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 80) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 5",16);
    PID_init();
    X.now = X.target =  X.p9;
    Y.now = Y.target =  Y.p9;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while(1)
    {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 75) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 75) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
}
/***********************************************************************************************************
发挥项目：3
***********************************************************************************************************/
void Fun7()
{
//    int Xsp,Ysp;
    int time_count = 0;
    int cc = 3;
    OLED_Clear();
    OLED_ShowString(0,0,(u8*)"more 3",16);
    OLED_ShowString(0,2,(u8*)"Move soround 5",16);
    OLED_ShowString(30,6,(u8*)"Part 2",16);
    //*****************************************************************************
    while(cc--)
    {
        PID_init();
        X.now = X.target =  X.p5 + 8;
        Y.now = Y.target =  Y.p5 ;
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        pidx.SetValue = X.target;
        pidy.SetValue = Y.target;
        time_count = 0;
        while(1)
        {
            time_count ++;
            key_scan();
            pidx.ActualValue = X.now;
            pidy.ActualValue = Y.now;
            X.angle = (int)PID_realize(&pidx);
            Y.angle = (int)PID_realize(&pidy);
            X.anglewrite = 500  + (X.speed * 60) - X.angle;
            Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
            if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
            if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
            if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
            if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
            SetServo(ID1,X.anglewrite);
            SetServo(ID2,Y.anglewrite);
            if(time_count > 2500)
                break;
            if(EXIT)
            {
                OLED_Clear();
                break;
            }
        }
        PID_init();
        X.now = X.target =  X.p5 + 8;
        Y.now = Y.target =  Y.p5 ;
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        pidx.SetValue = X.target;
        pidy.SetValue = Y.target;
        OLED_ShowString(30,6,(u8*)"Part 1",16);
        while(1)
        {
            time_count ++;
            key_scan();
            pidx.ActualValue = X.now;
            pidy.ActualValue = Y.now;
            X.angle = (int)PID_realize(&pidx);
            Y.angle = (int)PID_realize(&pidy);
            X.anglewrite = 500  + (X.speed * 60) - X.angle;
            Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
            if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
            if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
            if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
            if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
            SetServo(ID1,X.anglewrite);
            SetServo(ID2,Y.anglewrite);
            if(time_count > 2500)
                break;
            if(EXIT)
            {
                OLED_Clear();
                break;
            }
        }
        OLED_ShowString(30,6,(u8*)"Part 2",16);
        PID_init();
        X.now = X.target =  X.p5 ;
        Y.now = Y.target =  Y.p5 - 8;
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        pidx.SetValue = X.target;
        pidy.SetValue = Y.target;
        time_count = 0;
        while(1)
        {
            time_count ++;
            key_scan();
            pidx.ActualValue = X.now;
            pidy.ActualValue = Y.now;
            X.angle = (int)PID_realize(&pidx);
            Y.angle = (int)PID_realize(&pidy);
            X.anglewrite = 500  + (X.speed * 60) - X.angle;
            Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
            if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
            if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
            if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
            if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
            SetServo(ID1,X.anglewrite);
            SetServo(ID2,Y.anglewrite);
            if(time_count > 2500)
                break;
            if(EXIT)
            {
                OLED_Clear();
                break;
            }
        }
        OLED_ShowString(30,6,(u8*)"Part 3",16);
        PID_init();
        X.now = X.target =  X.p5 - 8 ;
        Y.now = Y.target =  Y.p5 ;
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        pidx.SetValue = X.target;
        pidy.SetValue = Y.target;
        time_count = 0;
        while(1)
        {
            time_count ++;
            key_scan();
            pidx.ActualValue = X.now;
            pidy.ActualValue = Y.now;
            X.angle = (int)PID_realize(&pidx);
            Y.angle = (int)PID_realize(&pidy);
            X.anglewrite = 500  + (X.speed * 80) - X.angle;
            Y.anglewrite = 500  + (Y.speed * 80) - Y.angle;
            if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
            if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
            if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
            if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
            SetServo(ID1,X.anglewrite);
            SetServo(ID2,Y.anglewrite);
            if(time_count > 3000)
                break;
            if(EXIT)
            {
                OLED_Clear();
                break;
            }
        }
    }
//*****************************************************************************
    OLED_ShowString(30,6,(u8*)"Part 5",16);
    PID_init();
    X.now = X.target =  X.p9;
    Y.now = Y.target =  Y.p9;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while(1)
    {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 75) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 75) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
//*****************************************************************************
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
}
void gg()
{
    uint32_t time_count = 0;
    OLED_Clear();
    OLED_ShowString(0,0,(u8*)"More 1",16);
    OLED_ShowString(0,2,(u8*)"Move by points",16);
    OLED_ShowString(0,2,(u8*)"A > B > C > D",16);
    PID_init();
    X.now = X.target =  X.p2;
    Y.now = Y.target =  Y.p2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    OLED_ShowString(30,6,(u8*)"Part 1",16);
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 2",16);
    PID_init();
    X.now = X.target =  (X.p2+X.p6)/2;
    Y.now = Y.target =  (Y.p2+Y.p6)/2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 3",16);
    PID_init();
    X.now = X.target = X.p6;
    Y.now = Y.target = Y.p6;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 80) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 80) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 3000)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    OLED_ShowString(30,6,(u8*)"Part 4",16);
    PID_init();
    X.now = X.target =  (X.p9+X.p6)/2;
    Y.now = Y.target =  (Y.p9+Y.p6)/2;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    time_count = 0;
    while(1)
    {
        time_count ++;
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 80) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 80) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(time_count > 2500)
            break;
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
//*****************************************************************************
    OLED_ShowString(30,6,(u8*)"Part 5",16);
    PID_init();
    X.now = X.target =  X.p9;
    Y.now = Y.target =  Y.p9;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while(1)
    {
        key_scan();
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
        X.anglewrite = 500  + (X.speed * 75) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 75) - Y.angle;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
    LobotSerialServoMove(ID1,500,500);
    LobotSerialServoMove(ID2,500,500);
}
/***********************************************************************************************************
发挥项目：4
***********************************************************************************************************/
void Fun8()
{
    OLED_Clear();
    OLED_ShowString(0,2,(u8*)"stay on 2 in 5s.",16);
    PID_init();
    X.now = X.target =  63;
    Y.now = Y.target =  60;
    pidx.ActualValue = X.now;
    pidy.ActualValue = Y.now;
    pidx.SetValue = X.target;
    pidy.SetValue = Y.target;
    while(1)
    {
        key_scan();
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
}
/***********************************************************************************************************
PID测试
***********************************************************************************************************/
void Pidtest()
{
//	int X_diff,Y_diff;
    OLED_Clear();
    OLED_ShowString(80,0,(u8*)"X   Y",16);
    OLED_ShowString(0, 2,(u8*)" Now",16);
    OLED_ShowString(0, 4,(u8*)"speed",16);
    OLED_ShowString(0, 6,(u8*)"AngleWrt",16);
    while(1)
    {
        key_scan();
//		if(UP)
//			X.now ++;
//		if(DOWN)
//			X.now --;
//		if(RIGHT)
//			Y.now ++;
//		if(LEFT)
//			Y.now --;
        pidx.ActualValue = X.now;
        pidy.ActualValue = Y.now;
        X.angle = (int)PID_realize(&pidx);
        Y.angle = (int)PID_realize(&pidy);
//		X.speedfb = (int)PID_realize(&pidxv);
//		Y.speedfb = (int)PID_realize(&pidyv);
//		X_diff = X.now - X.target;
//		Y_diff = Y.now - Y.target;
        X.anglewrite = 500  + (X.speed * 60) - X.angle;
        Y.anglewrite = 500  + (Y.speed * 60) - Y.angle;
//		X.anglewrite = 500  + X.speed * 80;
//		Y.anglewrite = 500  + Y.speed * 80;
        if(X.anglewrite > X_angle_up)   X.anglewrite = X_angle_up;
        if(X.anglewrite < X_angle_down) X.anglewrite = X_angle_down;
        if(Y.anglewrite > X_angle_up)   Y.anglewrite = X_angle_up;
        if(Y.anglewrite < X_angle_down) Y.anglewrite = X_angle_down;
        SetServo(ID1,X.anglewrite);
        SetServo(ID2,Y.anglewrite);
//		OLED_ShowNum( 72,2,X.now,3,16);
//		OLED_ShowNum( 72,4,X.speed,3,16);
//		OLED_ShowNum( 72,6,X.anglewrite,3,16);
//		OLED_ShowNum(104,2,Y.now,3,16);
//		OLED_ShowNum(104,4,Y.speed,3,16);
//		OLED_ShowNum(104,6,Y.anglewrite,3,16);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
}
void ShowBall()
{
    OLED_Clear();
    OLED_ShowString(0, 0,(u8*)"====ShowBall====", 16);
    OLED_ShowString(0, 2,(u8*)" X:", 16);
    OLED_ShowString(0, 4,(u8*)" Y:", 16);
    while(1)
    {
        key_scan();
        if(F1)
        {
            X.target = X.now;
            Y.target = Y.now;
            pidx.SetValue = X.now;
            pidy.SetValue = Y.now;
            OLED_ShowNum(80,2,X.target,5,16);
            OLED_ShowNum(80,4,Y.target,5,16);
        }
        OLED_ShowNum(32,2,X.now,5,16);
        OLED_ShowNum(32,4,Y.now,5,16);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
}
void SetPoint()
{
    uint8_t pointItem = 1;
    OLED_Clear();
    OLED_ShowString(0, 0,(u8*)"====SetPoint====", 16);
    OLED_ShowString(0, 2,(u8*)"Set:     X   Y", 16);
    OLED_ShowString(0, 4,(u8*)"Value", 16);
    OLED_ShowString(0, 6,(u8*)"Ball", 16);
    while(1)
    {
        key_scan();
        OLED_ShowNum(40,6,X.now,5,16);
        OLED_ShowNum(88,6,Y.now,5,16);
        if((UP||LEFT) && pointItem > 1)
            pointItem --;
        if((DOWN||RIGHT) && pointItem < 9)
            pointItem ++;
        switch(pointItem)
        {
        case 1:
            OLED_ShowString(32, 2,(u8*)"P1", 16);
            OLED_ShowNum(40,4,X.p1,5,16);
            OLED_ShowNum(88,4,Y.p1,5,16);
            break;
        case 2:
            OLED_ShowString(32, 2,(u8*)"P2", 16);
            OLED_ShowNum(40,4,X.p2,5,16);
            OLED_ShowNum(88,4,Y.p2,5,16);
            break;
        case 3:
            OLED_ShowString(32, 2,(u8*)"P3", 16);
            OLED_ShowNum(40,4,X.p3,5,16);
            OLED_ShowNum(88,4,Y.p3,5,16);
            break;
        case 4:
            OLED_ShowString(32, 2,(u8*)"P4", 16);
            OLED_ShowNum(40,4,X.p4,5,16);
            OLED_ShowNum(88,4,Y.p4,5,16);
            break;
        case 5:
            OLED_ShowString(32, 2,(u8*)"P5", 16);
            OLED_ShowNum(40,4,X.p5,5,16);
            OLED_ShowNum(88,4,Y.p5,5,16);
            break;
        case 6:
            OLED_ShowString(32, 2,(u8*)"P6", 16);
            OLED_ShowNum(40,4,X.p6,5,16);
            OLED_ShowNum(88,4,Y.p6,5,16);
            break;
        case 7:
            OLED_ShowString(32, 2,(u8*)"P7", 16);
            OLED_ShowNum(40,4,X.p7,5,16);
            OLED_ShowNum(88,4,Y.p7,5,16);
            break;
        case 8:
            OLED_ShowString(32, 2,(u8*)"P8", 16);
            OLED_ShowNum(40,4,X.p8,5,16);
            OLED_ShowNum(88,4,Y.p8,5,16);
            break;
        case 9:
            OLED_ShowString(32, 2,(u8*)"P9", 16);
            OLED_ShowNum(40,4,X.p9,5,16);
            OLED_ShowNum(88,4,Y.p9,5,16);
            break;
        default:
            break;
        }
        if(CONF)
        {
            switch(pointItem)
            {
            case 1:
                X.p1 = X.now;
                Y.p1 = Y.now;
                break;
            case 2:
                X.p2 = X.now;
                Y.p2 = Y.now;
                break;
            case 3:
                X.p3 = X.now;
                Y.p3 = Y.now;
                break;
            case 4:
                X.p4 = X.now;
                Y.p4 = Y.now;
                break;
            case 5:
                X.p5 = X.now;
                Y.p5 = Y.now;
                break;
            case 6:
                X.p6 = X.now;
                Y.p6 = Y.now;
                break;
            case 7:
                X.p7 = X.now;
                Y.p7 = Y.now;
                break;
            case 8:
                X.p8 = X.now;
                Y.p8 = Y.now;
                break;
            case 9:
                X.p9 = X.now;
                Y.p9 = Y.now;
                break;
            default:
                break;
            }
        }
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
}
void SetPID()
{
    OLED_Clear();
    OLED_ShowString(0, 0,(u8*)"=====SetPID=====", 16);
    while(1)
    {
        key_scan();
        if(UP)
            X.now += 10;
        if(DOWN)
            X.now -= 10;
        if(RIGHT)
            X.now ++;
        if(LEFT)
            X.now --;
        OLED_ShowNum(40,4,X.now,5,16);
        OLED_ShowNum(88,4,Y.now,5,16);
        if(EXIT)
        {
            OLED_Clear();
            break;
        }
    }
}
