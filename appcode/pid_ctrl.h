#ifndef _PID_CTRL_H
#define _PID_CTRL_H
#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
//#include "include.h"

enum {
    PIDROLL,
    PIDPITCH,
    PIDYAW,
    PIDITEMS
};
typedef struct
{
	xyz_f_t err;
	xyz_f_t err_old;
	xyz_f_t err_i;
	xyz_f_t eliminate_I;
	xyz_f_t err_d;
	xyz_f_t damp;
	xyz_f_t out;
	pid_t 	PID[PIDITEMS];
	xyz_f_t err_weight;
	float FB;

}ctrl_t;

typedef struct
{float x;
float y;
}_exp;



void CTRL_2(float T,float exp_x,float exp_y,float now_x,float now_y);
void CTRL_1(float T);










#endif
