#include "motor.h"
#include "tim.h"
#include <stdlib.h>

int motor_pwm_L = 0, motor_pwm_R = 0;

float Velcity_Kp=20,  Velcity_Ki=5,  Velcity_Kd; //����ٶ�PID����

uint8_t motor_init(void){
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_GPIO_WritePin(MOTOR_STBY_GPIO_Port,MOTOR_STBY_Pin,GPIO_PIN_SET);//ʹ�ܵ������
	return 0;
}

void motor_set_pwm(uint8_t motor_label, int raw_pwm){
	uint16_t siqu = 400;
	uint16_t dutyCycle = 0;
	
	if(motor_label == MOTOR_A){
		if(raw_pwm >= 0){
			 HAL_GPIO_WritePin(MOTOR_AIN1_GPIO_Port,MOTOR_AIN1_Pin,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(MOTOR_AIN2_GPIO_Port,MOTOR_AIN2_Pin,GPIO_PIN_SET);
		}else{
			 HAL_GPIO_WritePin(MOTOR_AIN1_GPIO_Port,MOTOR_AIN1_Pin,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(MOTOR_AIN2_GPIO_Port,MOTOR_AIN2_Pin,GPIO_PIN_RESET);
			 raw_pwm = -raw_pwm;
		}
		dutyCycle = raw_pwm+siqu;
		if(dutyCycle < MAX_PERIOD){
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,dutyCycle);
		}else{
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,MAX_PERIOD);
		}
	}
	
	if(motor_label == MOTOR_B){
		if(raw_pwm >= 0){
			 HAL_GPIO_WritePin(MOTOR_BIN1_GPIO_Port,MOTOR_BIN1_Pin,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(MOTOR_BIN2_GPIO_Port,MOTOR_BIN2_Pin,GPIO_PIN_RESET);
		}else{
			 HAL_GPIO_WritePin(MOTOR_BIN1_GPIO_Port,MOTOR_BIN1_Pin,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(MOTOR_BIN2_GPIO_Port,MOTOR_BIN2_Pin,GPIO_PIN_SET);
			 raw_pwm = -raw_pwm;
		}
		dutyCycle = raw_pwm+siqu;
		if(dutyCycle < MAX_PERIOD){
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,dutyCycle);
		}else{
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,MAX_PERIOD);
		}
	}	
	
}

void motor_stop(){
	HAL_GPIO_WritePin(MOTOR_AIN1_GPIO_Port,MOTOR_AIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_AIN2_GPIO_Port,MOTOR_AIN2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_BIN1_GPIO_Port,MOTOR_BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_BIN2_GPIO_Port,MOTOR_BIN2_Pin,GPIO_PIN_RESET);
}


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

