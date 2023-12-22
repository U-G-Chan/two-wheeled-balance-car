#include "motor.h"
#include "tim.h"
#include <stdlib.h>

int motor_pwm_L = 0, motor_pwm_R = 0;

float Velcity_Kp=20,  Velcity_Ki=5,  Velcity_Kd; //相关速度PID参数

uint8_t motor_init(void){
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_GPIO_WritePin(MOTOR_STBY_GPIO_Port,MOTOR_STBY_Pin,GPIO_PIN_SET);//使能电机驱动
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

