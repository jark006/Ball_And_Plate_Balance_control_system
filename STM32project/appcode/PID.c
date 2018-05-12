


#include "PID.h"
//X / Y ÖáPID
//#define Px_value   0.2
//#define Ix_value   0.015
//#define Dx_value   0.2

//#define Py_value   0.2
//#define Iy_value   0.015
//#define Dy_value   0.2

#define Pa   2.8
#define Ia   0.003
#define Da   0

#define Px_value   Pa
#define Ix_value   Ia
#define Dx_value   Da

#define Py_value   Pa
#define Iy_value   Ia
#define Dy_value   Da


//ËÙ¶ÈPID
#define Pv_value   40
#define Iv_value   0
#define Dv_value   0




PID pidx,pidy,pidxv,pidyv;

void PID_init()
{
	printf("PID_init begin \r\n");
	
	pidx.SetValue=0.0;
	pidx.ActualValue=0.0;
	pidx.err=0.0;
	pidx.err_last=0.0;
	pidx.voltage=0.0;
	pidx.integral=0.0;
	pidx.Kp=Px_value;
	pidx.Ki=Ix_value;
	pidx.Kd=Dx_value;
	
	pidy.SetValue=0.0;
	pidy.ActualValue=0.0;
	pidy.err=0.0;
	pidy.err_last=0.0;
	pidy.voltage=0.0;
	pidy.integral=0.0;
	pidy.Kp=Py_value;
	pidy.Ki=Iy_value;
	pidy.Kd=Dy_value;
	
	pidxv.SetValue=0.0;
	pidxv.ActualValue=0.0;
	pidxv.err=0.0;
	pidxv.err_last=0.0;
	pidxv.voltage=0.0;
	pidxv.integral=0.0;
	pidxv.Kp=Pv_value;
	pidxv.Ki=Iv_value;
	pidxv.Kd=Dv_value;
	
	pidyv.SetValue=0.0;
	pidyv.ActualValue=0.0;
	pidyv.err=0.0;
	pidyv.err_last=0.0;
	pidyv.voltage=0.0;
	pidyv.integral=0.0;
	pidyv.Kp=Pv_value;
	pidyv.Ki=Iv_value;
	pidyv.Kd=Dv_value;
	
	
	printf("PID_init end \r\n");
}

//float PID_realize(PID pid,float value)
//{
//	pid.SetValue = value;
//	pid.err = pid.SetValue - pid.ActualValue;
//	pid.integral += pid.err;
//	pid.voltage = pid.Kp * pid.err + pid.Ki * pid.integral+  pid.Kd * (pid.err - pid.err_last);
//	pid.err_last = pid.err;
//	pid.ActualValue = pid.voltage*1.0;
//	return pid.ActualValue;
//}

float PID_realize(PID *pid)
{
	//pid->SetValue = value;
	pid->err = pid->SetValue - pid->ActualValue;
	pid->integral += pid->err;
	pid->voltage = pid->Kp * pid->err + pid->Ki * pid->integral+  pid->Kd * (pid->err - pid->err_last);
	pid->err_last = pid->err;
	pid->ActualValue = pid->voltage*1.0;
	return pid->ActualValue;
}


float myPID_realize(PID *pid)
{
	//pid->SetValue = value;
	pid->err = pid->SetValue - pid->ActualValue;
	//pid->integral += pid->err;
	pid->voltage = pid->Kp * pid->err ;//+ pid->Ki * pid->integral+  pid->Kd * (pid->err - pid->err_last);
	//pid->err_last = pid->err;
	pid->ActualValue = pid->voltage*1.0;
	return pid->voltage;
}

