#include "motor_encoder.h"
#include "motor.h"
#include "tim.h"

static int MOTOR_ENCODER_A,MOTOR_ENCODER_B;

uint8_t motor_encoder_update(uint8_t motor_label){
	if(motor_label == MOTOR_A){
		MOTOR_ENCODER_A =  (short)__HAL_TIM_GET_COUNTER(&htim3);
		__HAL_TIM_SET_COUNTER(&htim3,0);
		return HAL_OK;	
		
	}
	if(motor_label == MOTOR_B){
		MOTOR_ENCODER_B = (short) __HAL_TIM_GET_COUNTER(&htim4);
		__HAL_TIM_SET_COUNTER(&htim4,0);
		return HAL_OK;		
	}
	return HAL_ERROR;
}

void motor_encoder_read(int *encoder_left,int *encoder_right){
	*encoder_right =  (short)__HAL_TIM_GET_COUNTER(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3,0);
	
	*encoder_left = -(short) __HAL_TIM_GET_COUNTER(&htim4);
	__HAL_TIM_SET_COUNTER(&htim4,0);
}