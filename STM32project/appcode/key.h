#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"

#define K1 PBout(4)
#define K2 PBout(5)
#define K3 PBout(6)
#define K4 PBin(9)
#define K5 PBin(8)
#define K6 PBin(7)

#define keyclr  keynum =0
#define Pressed keynum!=0
#define K(n)    keynum==(n)
#define F1      keynum==1
#define F2      keynum==2
#define F3      keynum==3
#define CONF    keynum==4
#define UP      keynum==5
#define EXIT    keynum==6
#define LEFT    keynum==7
#define DOWN    keynum==8
#define RIGHT   keynum==9


#define DELAY 10

extern u8 keynum;
void Init_KEY(void);
void key_scan(void);

#endif


