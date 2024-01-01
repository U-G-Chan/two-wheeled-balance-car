#include "control.h"

#include "car_mpu6050.h"
#include "motor.h"
#include "motor_encoder.h"
#include "usart.h"

#define ZHONGZHI 0
extern float Angle_Balance,Gyro_Balance;           //ƽ����� ƽ�������� ת��������

int BALANCE_PWM, VELOCITY_PWM,ANGLE_PWM;
float Velcity_Kp=20,  Velcity_Ki=5,  Velcity_Kd; //����ٶ�PID����

float Balance_Angle_Kp=100.0,   Balance_Angle_Ki=0,		 Balance_Angle_Kd=0.40;
float Balance_Velocity_Kp=50.0,Balance_Velocity_Ki=0.25,Balance_Velocity_Kd=0;

/**************************************************************************
�������ܣ��ٶȱջ�PID����(ʵ��ΪPI����)
��ڲ�����Ŀ���ٶ� ��ǰ�ٶ�
����  ֵ���ٶȿ���ֵ
��������ʽ��ɢPID��ʽ 
ControlVelocity+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
ControlVelocity�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
ControlVelocity+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
		int Bias;  //������ر���
		static int ControlVelocity, Last_bias; //��̬�������������ý�������ֵ��Ȼ����
		
		Bias=TargetVelocity-CurrentVelocity; //���ٶ�ƫ��
		
		ControlVelocity+=Velcity_Kp*(Bias-Last_bias)+Velcity_Ki*Bias;  //����ʽPI������
                                                                   //Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
	                                                                 //Velcity_Ki*Bias             �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
		Last_bias=Bias;	
		return ControlVelocity; //�����ٶȿ���ֵ
}

int control_balance_pwm_angle(float Angle, float Gyro){
	float Bias=Angle-ZHONGZHI;//���ƽ��ĽǶ���ֵ �ͻ�е���
	int balance= Balance_Angle_Kp*Bias+Gyro*Balance_Angle_Kd;//����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	return balance;
}

int control_balance_pwm_velocity(int encoder_left,int encoder_right){
	static float Velocity,Encoder_Least,Encoder,Movement;
	static float Encoder_Integral;
	//=============�ٶ�PI������=======================//	
	Encoder_Least =(encoder_left+encoder_right);                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
	Encoder *= 0.7;		                                                //===һ�׵�ͨ�˲���       
	Encoder += Encoder_Least*0.3;	                                    //===һ�׵�ͨ�˲���    
	Encoder_Integral += Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
	Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
	if(Encoder_Integral>15000)  	Encoder_Integral=15000;             //===�����޷�
	if(Encoder_Integral<-15000) 	Encoder_Integral=-15000;            //===�����޷�	
	Velocity=Encoder*Balance_Velocity_Kp+Encoder_Integral*Balance_Velocity_Ki;                          //===�ٶȿ���	
	//message("encoder_left=%d  encoder_right=%d\r\n",encoder_left,encoder_right);
	//message("Encoder=%f  Encoder_Integral=%f\r\n",Encoder,Encoder_Integral);
	return Velocity;
} 

uint8_t control_state_test(float angle){
	if(angle<-40||angle>40){//��Ǵ���40�ȹرյ��
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