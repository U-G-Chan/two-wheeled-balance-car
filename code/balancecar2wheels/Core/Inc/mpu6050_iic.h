#ifndef __MPU6050_IIC_H__
#define __MPU6050_IIC_H__

#include "stm32f1xx_hal.h"

#define MPU6050_IIC_MAX_DELAY 1000

#define MPU6050_SLAVE_ADDRESS  (0x68<<1)      //MPU6050Æ÷¼þ¶ÁµØÖ·

uint8_t mpu6050_iic_write(uint8_t reg, uint8_t* pData, uint16_t size);
uint8_t mpu6050_iic_read(uint8_t reg, uint8_t* pBuf, uint16_t size);

uint8_t mpu6050_iic_write_byte(uint8_t reg, uint8_t* pData);
uint8_t mpu6050_iic_read_byte(uint8_t reg, uint8_t* pData);

uint8_t mpu6050_iic_write_bits(uint8_t reg, uint8_t bit_start, uint8_t length, uint8_t data);

uint8_t mpu6050_iic_write_bit(uint8_t reg, uint8_t bit_num, uint8_t data);

int my_i2c_write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int my_i2c_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif