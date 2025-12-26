/*******************************************************************************
舵机控制程序
*******************************************************************************/

#include "servo.h"
#include "delay.h"
#include "stm32f10x.h"
#include "usart1.h"
#include <stdarg.h>
#include <string.h>

void LobotSerialWrite(uint8_t* buff, uint8_t len) {
    usart_write(USART2, buff, len);
}

// 宏函数 获得A的低八位
#define GET_LOW_BYTE(A) ((uint8_t)(A))

// 宏函数 获得A的高八位
#define GET_HIGH_BYTE(A) ((uint8_t)((A) >> 8))

// 宏函数 将高地八位合成为十六位
#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))

uint8_t LobotCheckSum(const uint8_t* buf) {
    uint16_t temp = 0;
    for (uint8_t i = 2; i < buf[3] + 2; i++) {
        temp += buf[i];
    }
    return (uint8_t)(~temp);
}

void LobotSerialServoSetID(uint8_t oldID, uint8_t newID) {
    uint8_t buf[7];
    buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
    buf[2] = oldID;
    buf[3] = 4;
    buf[4] = LOBOT_SERVO_ID_WRITE;
    buf[5] = newID;
    buf[6] = LobotCheckSum(buf);
    LobotSerialWrite(buf, 7);
}

void LobotSerialServoMove(uint8_t id, int16_t position, uint16_t time) {
    uint8_t buf[10];
    if (position < 0)
        position = 0;
    if (position > 1000)
        position = 1000;
    buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
    buf[2] = id;
    buf[3] = 7;
    buf[4] = LOBOT_SERVO_MOVE_TIME_WRITE;
    buf[5] = GET_LOW_BYTE(position);
    buf[6] = GET_HIGH_BYTE(position);
    buf[7] = GET_LOW_BYTE(time);
    buf[8] = GET_HIGH_BYTE(time);
    buf[9] = LobotCheckSum(buf);
    LobotSerialWrite(buf, 10);
}

void LobotSerialServoUnload(uint8_t id) {
    uint8_t buf[7];
    buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
    buf[2] = id;
    buf[3] = 4;
    buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
    buf[5] = 0;
    buf[6] = LobotCheckSum(buf);
    LobotSerialWrite(buf, 7);
}

void LobotSerialServoLoad(uint8_t id) {
    uint8_t buf[7];
    buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
    buf[2] = id;
    buf[3] = 4;
    buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
    buf[5] = 1;
    buf[6] = LobotCheckSum(buf);
    LobotSerialWrite(buf, 7);
}

// 舵机回正  回复到初始平衡状态
void ServoResetPosition() {
    LobotSerialServoMove(ID1, 500, 500);
    LobotSerialServoMove(ID2, 500, 500);
    delay_ms(500);
}
