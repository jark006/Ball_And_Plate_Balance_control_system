#include "pid.h"
#include <string.h>

#define Pa 2.8
#define Ia 0.003
#define Da 0

// X-Y ÖáPID
#define Px_value Pa
#define Ix_value Ia
#define Dx_value Da

#define Py_value Pa
#define Iy_value Ia
#define Dy_value Da

PID pidx, pidy;

void PID_init() {
    memset(&pidx, 0, sizeof(PID));
    memset(&pidy, 0, sizeof(PID));

    pidx.Kp = Px_value;
    pidx.Ki = Ix_value;
    pidx.Kd = Dx_value;

    pidy.Kp = Py_value;
    pidy.Ki = Iy_value;
    pidy.Kd = Dy_value;
}

int PID_realize(PID* pid) {
    pid->err = pid->SetValue - pid->ActualValue;
    pid->integral += pid->err;
    float controlOutput = pid->Kp * pid->err +
        pid->Ki * pid->integral +
        pid->Kd * (pid->err - pid->err_last);
    pid->err_last = pid->err;
    return (int)(controlOutput);
}
