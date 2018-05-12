#include "pid_ctrl.h"
_locat locat;
xyz_f_t except_A = {0,0,0};
xyz_f_t except_AS;
ctrl_t ctrl_1;
ctrl_t ctrl_2;

float g_old[ITEMS];

void CTRL_1(float T)  
{
	xyz_f_t EXP_LPF_TMP;
	/* 给期望（目标）角速度 */
	EXP_LPF_TMP.x = 60 *(ctrl_2.out.x/30);
	EXP_LPF_TMP.y = 60 *(ctrl_2.out.y/30);
	
	except_AS.x = EXP_LPF_TMP.x;
	except_AS.y = EXP_LPF_TMP.y;
	/* 期望角速度限幅 */
	except_AS.x = LIMIT(except_AS.x, -60,60 );
	except_AS.y = LIMIT(except_AS.y, -60,60 );

	/* 角速度直接微分（角加速度），负反馈可形成角速度的阻尼（阻碍角速度的变化）*/
	ctrl_1.damp.x = (mpu6050.Gyro_deg.x - g_old[A_X]) *( 0.002f/T );
	ctrl_1.damp.y = (mpu6050.Gyro_deg.y - g_old[A_Y]) *( 0.002f/T );
	/* 角速度误差 */
	ctrl_1.err.x =  ( except_AS.x + mpu6050.Gyro_deg.x ) ;
	ctrl_1.err.y =  ( except_AS.y + mpu6050.Gyro_deg.y ) ;   
	/* 角速度误差权重 */
	ctrl_1.err_weight.x = ABS(ctrl_1.err.x/60);
	ctrl_1.err_weight.y = ABS(ctrl_1.err.y/60);
	/* 角速度微分 */
	ctrl_1.err_d.x = ( ctrl_1.PID[PIDROLL].kd  *( -1 *ctrl_1.damp.x) *( 0.002f/T ) );
	ctrl_1.err_d.y = ( ctrl_1.PID[PIDPITCH].kd *( -1 *ctrl_1.damp.y) *( 0.002f/T ) );

	/* 角速度误差积分 */
	ctrl_1.err_i.x += ctrl_1.PID[PIDROLL].ki  *(ctrl_1.err.x) *T; 
	ctrl_1.err_i.y += ctrl_1.PID[PIDPITCH].ki *(ctrl_1.err.y) *T;

	/* 角速度误差积分限幅 */
	ctrl_1.err_i.x = LIMIT( ctrl_1.err_i.x, -30,30 );
	ctrl_1.err_i.y = LIMIT( ctrl_1.err_i.y, -30,30 );
	
	/* 角速度PID输出 */
	ctrl_1.out.x = ctrl_1.FB *LIMIT((0.45f + 0.55f*ctrl_2.err_weight.x),0,1)*except_AS.x + ( 1 - ctrl_1.FB ) *ctrl_1.PID[PIDROLL].kp  *( ctrl_1.err.x + ctrl_1.err_d.x + ctrl_1.err_i.x ) ;
	ctrl_1.out.y = ctrl_1.FB *LIMIT((0.45f + 0.55f*ctrl_2.err_weight.y),0,1)*except_AS.y + ( 1 - ctrl_1.FB ) *ctrl_1.PID[PIDPITCH].kp *( ctrl_1.err.y + ctrl_1.err_d.y + ctrl_1.err_i.y ) ;
					


	ctrl_1.err_old.x = ctrl_1.err.x;
	ctrl_1.err_old.y = ctrl_1.err.y;

	g_old[A_X] = mpu6050.Gyro_deg.x ;
	g_old[A_Y] = mpu6050.Gyro_deg.y ;

}
void CTRL_2(float T,float exp_x,float exp_y,float now_x,float now_y)
{
//=========================== 期望距离 ========================================
	except_A.x  =   exp_x - now_x ;
	except_A.y  =   exp_y - now_y;


  /* 得到距离误差 */
	ctrl_2.err.x =  except_A.x  ;//限制每次调节误差最大为5cm
	ctrl_2.err.y =  except_A.y  ;

	/* 计算距离误差权重 */
	ctrl_2.err_weight.x = ABS(ctrl_2.err.x)/5;
	ctrl_2.err_weight.y = ABS(ctrl_2.err.y)/5;

	/* 距离误差微分（跟随误差曲线变化）*/
	ctrl_2.err_d.x = ctrl_2.PID[PIDROLL].kd  *(ctrl_2.err.x - ctrl_2.err_old.x) *( 0.005f/T ) *( 0.65f + 0.35f *ctrl_2.err_weight.x );
	ctrl_2.err_d.y = ctrl_2.PID[PIDPITCH].kd *(ctrl_2.err.y - ctrl_2.err_old.y) *( 0.005f/T ) *( 0.65f + 0.35f *ctrl_2.err_weight.y );

	/* 距离误差积分 */
	ctrl_2.err_i.x += ctrl_2.PID[PIDROLL].ki  *ctrl_2.err.x *T;
	ctrl_2.err_i.y += ctrl_2.PID[PIDPITCH].ki *ctrl_2.err.y *T;

	/* 距离误差积分限幅 */
	if(ctrl_2.err_i.x>8||ctrl_2.err_i.x<-8)//如果积分大于5取消积分
		ctrl_2.err_i.x=0;
	else
	ctrl_2.err_i.x = LIMIT( ctrl_2.err_i.x, -8,8 );
  if(ctrl_2.err_i.y>8||ctrl_2.err_i.y<-8)
	ctrl_2.err_i.y=0;
	else
	ctrl_2.err_i.y = LIMIT( ctrl_2.err_i.y, -8,8 );

	/* 对用于计算比例项输出的距离误差限幅 */
	ctrl_2.err.x = LIMIT( ctrl_2.err.x, -10, 10 );
	ctrl_2.err.y = LIMIT( ctrl_2.err.y, -10, 10 );
	/* 距离PID输出 */
	ctrl_2.out.x = ctrl_2.PID[PIDROLL].kp  *( ctrl_2.err.x + ctrl_2.err_d.x + ctrl_2.err_i.x );	
	ctrl_2.out.y = ctrl_2.PID[PIDPITCH].kp *( ctrl_2.err.y + ctrl_2.err_d.y + ctrl_2.err_i.y );  

	/* 记录历史数据 */	
	ctrl_2.err_old.x = ctrl_2.err.x;
	ctrl_2.err_old.y = ctrl_2.err.y;


}
