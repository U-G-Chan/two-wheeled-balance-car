#include "control.h"

#include "car_mpu6050.h"
#include "motor.h"
#include "motor_encoder.h"
#include "usart.h"

#define ZHONGZHI 0
extern float Angle_Balance,Gyro_Balance;           //平衡倾角 平衡陀螺仪 转向陀螺仪

int BALANCE_PWM, VELOCITY_PWM,ANGLE_PWM;
float Velcity_Kp=20,  Velcity_Ki=5,  Velcity_Kd; //相关速度PID参数

float Balance_Angle_Kp=100.0,   Balance_Angle_Ki=0,		 Balance_Angle_Kd=0.40;
float Balance_Velocity_Kp=50.0,Balance_Velocity_Ki=0.25,Balance_Velocity_Kd=0;

/**************************************************************************
函数功能：速度闭环PID控制(实际为PI控制)
入口参数：目标速度 当前速度
返回  值：速度控制值
根据增量式离散PID公式 
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
ControlVelocity代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
		int Bias;  //定义相关变量
		static int ControlVelocity, Last_bias; //静态变量，函数调用结束后其值依然存在
		
		Bias=TargetVelocity-CurrentVelocity; //求速度偏差
		
		ControlVelocity+=Velcity_Kp*(Bias-Last_bias)+Velcity_Ki*Bias;  //增量式PI控制器
                                                                   //Velcity_Kp*(Bias-Last_bias) 作用为限制加速度
	                                                                 //Velcity_Ki*Bias             速度控制值由Bias不断积分得到 偏差越大加速度越大
		Last_bias=Bias;	
		return ControlVelocity; //返回速度控制值
}

int control_balance_pwm_angle(float Angle, float Gyro){
	float Bias=Angle-ZHONGZHI;//求出平衡的角度中值 和机械相关
	int balance= Balance_Angle_Kp*Bias+Gyro*Balance_Angle_Kd;//计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	return balance;
}

int control_balance_pwm_velocity(int encoder_left,int encoder_right){
	static float Velocity,Encoder_Least,Encoder,Movement;
	static float Encoder_Integral;
	//=============速度PI控制器=======================//	
	Encoder_Least =(encoder_left+encoder_right);                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
	Encoder *= 0.7;		                                                //===一阶低通滤波器       
	Encoder += Encoder_Least*0.3;	                                    //===一阶低通滤波器    
	Encoder_Integral += Encoder;                                       //===积分出位移 积分时间：10ms
	Encoder_Integral=Encoder_Integral-Movement;                       //===接收遥控器数据，控制前进后退
	if(Encoder_Integral>15000)  	Encoder_Integral=15000;             //===积分限幅
	if(Encoder_Integral<-15000) 	Encoder_Integral=-15000;            //===积分限幅	
	Velocity=Encoder*Balance_Velocity_Kp+Encoder_Integral*Balance_Velocity_Ki;                          //===速度控制	
	//message("encoder_left=%d  encoder_right=%d\r\n",encoder_left,encoder_right);
	//message("Encoder=%f  Encoder_Integral=%f\r\n",Encoder,Encoder_Integral);
	return Velocity;
} 

uint8_t control_state_test(float angle){
	if(angle<-40||angle>40){//倾角大于40度关闭电机
				motor_stop();
				return HAL_ERROR;
	}else{
		return HAL_OK;
	}
	return 0;
}

uint8_t control_balance(){
	
	//MOTOR_ENCODER_B = motor_encoder_read(MOTOR_B);
	//mpu6050_get_angle();
	static float forward_angle = 0;
	static short forward_gyr = 0;
	mpu6050_get_balnace_feedback(&forward_angle,&forward_gyr);
	if(control_state_test(forward_angle) == HAL_OK){
		//printf("forward_angle=%.1f,forward_gyr=%d\r\n",forward_angle,forward_gyr);
		ANGLE_PWM  = control_balance_pwm_angle(forward_angle,(float)forward_gyr);
		printf("ANGLE_PWM=%d\r\n",ANGLE_PWM);
		//VELOCITY_PWM = Velocity_FeedbackControl(0, MOTOR_ENCODER_A);
		//VELOCITY_PWM = control_balance_pwm_velocity(0,MOTOR_ENCODER_A);
		//BALANCE_PWM =  ANGLE_PWM+VELOCITY_PWM;
		//message("Angle_Balance=%f,Gyro_Balance=%f,BALANCE_PWM=%d\r\n",Angle_Balance,Gyro_Balance,BALANCE_PWM);
		//message("Angle_Balance[%f],Gyro_Balance[%f],ANGLE_PWM[%d]\r\n",Angle_Balance,Gyro_Balance,ANGLE_PWM);
		//message("MOTOR_ENCODER_A[%d],VELOCITY_PWM[%d]\r\n",MOTOR_ENCODER_A,VELOCITY_PWM);
//		message("BALANCE_PWM[]")
		motor_set_pwm(MOTOR_A,-ANGLE_PWM);
		motor_set_pwm(MOTOR_B,-ANGLE_PWM);
		return HAL_OK;
	}else{
		return HAL_ERROR;
	}
}