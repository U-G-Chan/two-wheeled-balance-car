#include "motor_encoder.h"
#include "motor.h"
#include "tim.h"

int motor_encoder_read(uint8_t motor_label){
	int Encoder_TIM = 0;
	if(motor_label == MOTOR_A){
		Encoder_TIM =  __HAL_TIM_GET_COUNTER(&htim3);
		__HAL_TIM_SET_COUNTER(&htim3,0);		
	}
	if(motor_label == MOTOR_B){
		Encoder_TIM =  __HAL_TIM_GET_COUNTER(&htim4);
		__HAL_TIM_SET_COUNTER(&htim4,0);	
	}
	if(Encoder_TIM > 0xefff){
		Encoder_TIM=Encoder_TIM-0xffff;           //转化计数值为有方向的值，大于0正转，小于0反转。
	}
	return Encoder_TIM;
}     
