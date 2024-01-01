#ifndef __CONTROL_H__
#define __CONTROL_H__   

#include "stm32f1xx_hal.h"

extern float Angle_Balance,Gyro_Balance;


int Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity);
int Angle_FeedbackControl(float Angle, float Gyro);

int control_balance_pwm_angle(float Angle, float Gyro);
int control_balance_pwm_velocity(int encoder_left,int encoder_right);

uint8_t control_balance();
#endif

