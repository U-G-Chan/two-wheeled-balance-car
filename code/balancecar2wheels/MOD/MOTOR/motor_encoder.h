#ifndef __MOTOR_ENCODER_H__
#define __MOTOR_ENCODER_H__

#include "stm32f1xx_hal.h"

uint8_t motor_encoder_update(uint8_t motor_label);

void motor_encoder_read(int *encoder_left,int *encoder_right);



#endif