#ifndef __CONTROL_H__
#define __CONTROL_H__   

#include "stm32f1xx_hal.h"


int control_pwm_balance(float Angle, float Gyro);

int control_pwm_velocity(int encoder_left,int encoder_right);

int control_pwm_turn(int encoder_left,int encoder_right,float gyro);

uint8_t control_state_test(float angle);

uint8_t control_pwm(); 

#endif

