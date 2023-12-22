#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f1xx_hal.h"

#define MAX_PERIOD 6400

#define MOTOR_A 1
#define MOTOR_B 2

uint8_t motor_init(void);

void motor_set_pwm(uint8_t motor_label, int raw_pwm);
void motor_stop();
int Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity);

#endif