#include "control.h"

#include "car_mpu6050.h"
#include "motor.h"
#include "motor_encoder.h"
#include "usart.h"
#include <stdlib.h>

#define ZERO -1  //平衡角度，根据实际情况调整
#define ENCODER_INTEGRAL_LIMIT 1000

uint8_t Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=1; //蓝牙遥控相关的变量

//PID系数
// balance 110 0 0.5     velocity 20  20 / 200

// balance 135 0 1.8     velocity 35  35 / 200
static float Balance_Kp = 90,   Balance_Ki = 0,			 Balance_Kd = 1v                                                   .5;
static float Velocity_Kp = 20,	 Velocity_Ki = 20/200,   Velocity_Kd = 0;
static float Turn_Kp = 42,         Turn_Ki = 0,          Turn_Kd = 0;

//速度控制变量
static float velocity_target = 0;//目标速度
static float velocity_forward = 1,velocity_backward = 0; 


//直立控制PWM
int control_pwm_balance(float Angle, float Gyro){
	float Bias=Angle-ZERO;//求出平衡的角度中值 和机械相关
	int balance= Balance_Kp*Bias+Gyro*Balance_Kd;//计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数
	return -balance;
}


static float Velocity,Encoder_Least,Encoder,Movement = 0;
static float Encoder_Integral;
//速度控制PWM
int control_pwm_velocity(int encoder_left,int encoder_right){
	//printf("encoder_left=%d,encoder_right=%d\r\n",encoder_left,encoder_right);
	//Velocity_Ki = Velocity_Kp/200;

	//=============速度大小方向设置====================//
//	if(velocity_forward == 1){
//		Movement = velocity_target;
//	}else if(velocity_backward == 1){
//		Movement = -velocity_target;
//	}else{
//		Movement = 0;
//	}
	//=============速度PI控制器=======================//	
	Encoder_Least =(encoder_left+encoder_right) - Movement;             //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
	Encoder *= 0.5;		                                                	//===一阶低通滤波器       
	Encoder += Encoder_Least*0.5;	                                    	//===一阶低通滤波器    
	Encoder_Integral += Encoder;                                       	//===积分出位移 积分时间：10ms
	Encoder_Integral=Encoder_Integral ;                       	//===接收遥控器数据，控制前进后退
	if(Encoder_Integral >  ENCODER_INTEGRAL_LIMIT)  Encoder_Integral =  ENCODER_INTEGRAL_LIMIT;            	//===积分限幅
	if(Encoder_Integral < -ENCODER_INTEGRAL_LIMIT) 	Encoder_Integral = -ENCODER_INTEGRAL_LIMIT;            	//===积分限幅	
	Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki; 				  //===速度控制
	//printf("Encoder=%f,Encoder_Integral=%f\r\n",Encoder,Encoder_Integral);
	return Velocity;
}

//转向控制PWM
int control_pwm_turn(int encoder_left,int encoder_right,float gyro)//转向控制
{
    static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.7,Turn_Count;
	  float Turn_Amplitude=50/Flag_sudu;    
	  //=============遥控左右旋转部分=======================//
  	if(1==Flag_Left||1==Flag_Right)                      //这一部分主要是根据旋转前的速度调整速度的起始速度，增加小车的适应性
		{
			if(++Turn_Count==1)
			Encoder_temp=abs(encoder_left+encoder_right);
			Turn_Convert=50/Encoder_temp;
			if(Turn_Convert<0.4)Turn_Convert=0.4;
			if(Turn_Convert>1)Turn_Convert=1;
		}	
	  else
		{
			Turn_Convert=0.7;
			Turn_Count=0;
			Encoder_temp=0;
		}		
		if(1==Flag_Left)	           Turn_Target-=Turn_Convert;        //===接收转向遥控数据
		else if(1==Flag_Right)	     Turn_Target+=Turn_Convert;        //===接收转向遥控数据
		else Turn_Target=0;                                            //===接收转向遥控数据
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===转向速度限幅
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;   //===转向速度限幅
		if(Flag_Qian==1||Flag_Hou==1)  Turn_Kd=0.6;                         //===接收转向遥控数据直立行走的时候增加陀螺仪就纠正    
		else Turn_Kd=0;                                   
  	//=============转向PD控制器=======================//
		Turn=Turn_Target*Turn_Kp+gyro*Turn_Kd;                 //===结合Z轴陀螺仪进行PD控制
	  return Turn;
}


uint8_t control_state_test(float angle){
	if(angle<-60||angle>60){//倾角大于80度关闭电机
		Encoder_Integral = 0;		
		motor_stop();
		return HAL_ERROR;
	}else{
		return HAL_OK;
	}
	return 0;
}

//综合pwm控制
static int COMBINE_PWM,BALANCE_PWM,VELOCITY_PWM,TURN_PWM;

static int MOTOR_ENCODER_A,MOTOR_ENCODER_B;
static int encoder_left,encoder_right;

static float forward_angle = 0;
static short forward_gyr = 0;

uint8_t control_pwm(){
			//motor_encoder_update(MOTOR_A);
			//motor_encoder_update(MOTOR_B);
			//mpu6050_update_angle();
			//mpu6050_update_acc();
			//mpu6050_update_gyr();
	
	motor_encoder_read(&encoder_left, &encoder_right);
	mpu6050_get_balnace_feedback(&forward_angle,&forward_gyr); //读取当前角度
	
	
	if(control_state_test(forward_angle) == HAL_OK){
		BALANCE_PWM  = control_pwm_balance(forward_angle,forward_gyr);
		VELOCITY_PWM = control_pwm_velocity(encoder_left,encoder_right);
		COMBINE_PWM = BALANCE_PWM + VELOCITY_PWM;
		//printf("encoder_left=%d,encoder_right=%d\r\n",encoder_left,encoder_right);
		printf("angle=%.1f,BALANCE_PWM=%d,VELOCITY_PWM=%d\r\n",forward_angle,BALANCE_PWM,VELOCITY_PWM);
		motor_set_pwm(MOTOR_A,COMBINE_PWM);
		motor_set_pwm(MOTOR_B,COMBINE_PWM);
		return HAL_OK;
	}else{
		return HAL_ERROR;
	}
}