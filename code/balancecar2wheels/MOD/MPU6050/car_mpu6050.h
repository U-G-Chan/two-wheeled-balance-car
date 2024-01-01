#ifndef __CAR_MPU6050_H__
#define __CAR_MPU6050_H__

#include "stm32f1xx_hal.h"


uint8_t mpu6050_init(void);

uint8_t mpu6050_update_angle();

uint8_t mpu6050_update_acc();

uint8_t mpu6050_update_gyr();

void mpu6050_show_angle();

void mpu6050_get_balnace_feedback(float *forward_angle, short *forward_gyr);

#endif
