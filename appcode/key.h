#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"

#define F1      (keyValue==1)
#define F2      (keyValue==2)
#define F3      (keyValue==3)
#define CONF    (keyValue==4)
#define UP      (keyValue==5)
#define EXIT    (keyValue==6)
#define LEFT    (keyValue==7)
#define DOWN    (keyValue==8)
#define RIGHT   (keyValue==9)

extern u8 keyValue;

void Init_KEY(void);
void key_scan(void);

#endif


