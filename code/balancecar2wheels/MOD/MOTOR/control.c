#include "control.h"

#include "car_mpu6050.h"
#include "motor.h"
#include "motor_encoder.h"
#include "usart.h"
#include <stdlib.h>

#define ZERO -1  //ƽ��Ƕȣ�����ʵ���������
#define ENCODER_INTEGRAL_LIMIT 1000

uint8_t Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=1; //����ң����صı���

//PIDϵ��
// balance 110 0 0.5     velocity 20  20 / 200

// balance 135 0 1.8     velocity 35  35 / 200
static float Balance_Kp = 90,   Balance_Ki = 0,			 Balance_Kd = 1v                                                   .5;
static float Velocity_Kp = 20,	 Velocity_Ki = 20/200,   Velocity_Kd = 0;
static float Turn_Kp = 42,         Turn_Ki = 0,          Turn_Kd = 0;

//�ٶȿ��Ʊ���
static float velocity_target = 0;//Ŀ���ٶ�
static float velocity_forward = 1,velocity_backward = 0; 


//ֱ������PWM
int control_pwm_balance(float Angle, float Gyro){
	float Bias=Angle-ZERO;//���ƽ��ĽǶ���ֵ �ͻ�е���
	int balance= Balance_Kp*Bias+Gyro*Balance_Kd;//����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ��
	return -balance;
}


static float Velocity,Encoder_Least,Encoder,Movement = 0;
static float Encoder_Integral;
//�ٶȿ���PWM
int control_pwm_velocity(int encoder_left,int encoder_right){
	//printf("encoder_left=%d,encoder_right=%d\r\n",encoder_left,encoder_right);
	//Velocity_Ki = Velocity_Kp/200;

	//=============�ٶȴ�С��������====================//
//	if(velocity_forward == 1){
//		Movement = velocity_target;
//	}else if(velocity_backward == 1){
//		Movement = -velocity_target;
//	}else{
//		Movement = 0;
//	}
	//=============�ٶ�PI������=======================//	
	Encoder_Least =(encoder_left+encoder_right) - Movement;             //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
	Encoder *= 0.5;		                                                	//===һ�׵�ͨ�˲���       
	Encoder += Encoder_Least*0.5;	                                    	//===һ�׵�ͨ�˲���    
	Encoder_Integral += Encoder;                                       	//===���ֳ�λ�� ����ʱ�䣺10ms
	Encoder_Integral=Encoder_Integral ;                       	//===����ң�������ݣ�����ǰ������
	if(Encoder_Integral >  ENCODER_INTEGRAL_LIMIT)  Encoder_Integral =  ENCODER_INTEGRAL_LIMIT;            	//===�����޷�
	if(Encoder_Integral < -ENCODER_INTEGRAL_LIMIT) 	Encoder_Integral = -ENCODER_INTEGRAL_LIMIT;            	//===�����޷�	
	Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki; 				  //===�ٶȿ���
	//printf("Encoder=%f,Encoder_Integral=%f\r\n",Encoder,Encoder_Integral);
	return Velocity;
}

//ת�����PWM
int control_pwm_turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
    static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.7,Turn_Count;
	  float Turn_Amplitude=50/Flag_sudu;    
	  //=============ң��������ת����=======================//
  	if(1==Flag_Left||1==Flag_Right)                      //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
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
		if(1==Flag_Left)	           Turn_Target-=Turn_Convert;        //===����ת��ң������
		else if(1==Flag_Right)	     Turn_Target+=Turn_Convert;        //===����ת��ң������
		else Turn_Target=0;                                            //===����ת��ң������
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת���ٶ��޷�
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;   //===ת���ٶ��޷�
		if(Flag_Qian==1||Flag_Hou==1)  Turn_Kd=0.6;                         //===����ת��ң������ֱ�����ߵ�ʱ�����������Ǿ;���    
		else Turn_Kd=0;                                   
  	//=============ת��PD������=======================//
		Turn=Turn_Target*Turn_Kp+gyro*Turn_Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}


uint8_t control_state_test(float angle){
	if(angle<-60||angle>60){//��Ǵ���80�ȹرյ��
		Encoder_Integral = 0;		
		motor_stop();
		return HAL_ERROR;
	}else{
		return HAL_OK;
	}
	return 0;
}

//�ۺ�pwm����
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
	mpu6050_get_balnace_feedback(&forward_angle,&forward_gyr); //��ȡ��ǰ�Ƕ�
	
	
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