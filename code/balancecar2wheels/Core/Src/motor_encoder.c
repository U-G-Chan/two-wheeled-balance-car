#include "motor_encoder.h"
#include "motor.h"
#include "tim.h"



int motor_encoder_read(uint8_t motor_label){
	int Encoder_TIM;
	TIM_HandleTypeDef htim = htim3;
	if(motor_label == MOTOR_A){
		htim = htim3;
	}
	if(motor_label == MOTOR_B){
		 
		htim = htim4;
	}
	Encoder_TIM =  __HAL_TIM_GET_COUNTER(&htim);
	if(Encoder_TIM > 0xefff){
		Encoder_TIM=Encoder_TIM-0xffff;           //ת������ֵΪ�з����ֵ������0��ת��С��0��ת��
	}
	__HAL_TIM_SET_COUNTER(&htim,0);             //��ȡ����������
	return Encoder_TIM;
}     
