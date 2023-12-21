#include "motor.h"
#include "tim.h"
#include <stdlib.h>

int motor_pwm_L = 0, motor_pwm_R = 0;

uint8_t motor_init(void){
	
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	return 0;
}

void motor_set_pwm(uint8_t motor_label, int raw_pwm){
	uint16_t siqu = 400;
	uint16_t dutyCycle = 0;
	
	if(motor_label == MOTOR_A){
		if(raw_pwm >= 0){
			 HAL_GPIO_WritePin(MOTOR_AIN1_GPIO_Port,MOTOR_AIN1_Pin,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(MOTOR_AIN2_GPIO_Port,MOTOR_AIN2_Pin,GPIO_PIN_RESET);
		}else{
			 HAL_GPIO_WritePin(MOTOR_AIN1_GPIO_Port,MOTOR_AIN1_Pin,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(MOTOR_AIN2_GPIO_Port,MOTOR_AIN2_Pin,GPIO_PIN_SET);
			 raw_pwm = -raw_pwm;
		}
		dutyCycle = raw_pwm+siqu;
		if(dutyCycle < MAX_PERIOD){
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,dutyCycle);
		}else{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,MAX_PERIOD);
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

