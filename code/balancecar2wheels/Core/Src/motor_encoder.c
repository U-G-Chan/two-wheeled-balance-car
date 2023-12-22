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
		Encoder_TIM=Encoder_TIM-0xffff;           //转化计数值为有方向的值，大于0正转，小于0反转。
	}
	__HAL_TIM_SET_COUNTER(&htim,0);             //读取完后计数清零
	return Encoder_TIM;
}     
