#include "mpu6050_iic.h"
#include "i2c.h"


uint8_t mpu6050_iic_write(uint8_t reg, uint8_t* pData, uint16_t size){
	return HAL_I2C_Mem_Write(&hi2c1, MPU6050_SLAVE_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, pData, size, MPU6050_IIC_MAX_DELAY);
}

uint8_t mpu6050_iic_read(uint8_t reg, uint8_t* pBuf, uint16_t size){
	return  HAL_I2C_Mem_Read(&hi2c1, MPU6050_SLAVE_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, pBuf, size, MPU6050_IIC_MAX_DELAY);
}

uint8_t mpu6050_iic_write_byte(uint8_t reg, uint8_t* pData){
	 return mpu6050_iic_write(reg, pData, 1);
}

uint8_t mpu6050_iic_read_byte(uint8_t reg, uint8_t* pData){
	 return mpu6050_iic_read(reg, pData, 1);
}

uint8_t mpu6050_iic_write_bits(uint8_t reg, uint8_t bit_start, uint8_t length, uint8_t data){
	uint8_t byte = 0;
	if (mpu6050_iic_read_byte(reg, &byte) == HAL_OK) {
		uint8_t mask = (0xFF << (bit_start + 1)) | 0xFF >> ((8 - bit_start) + length - 1);
		data <<= (8 - length);
    data >>= (7 - bit_start);
    byte &= mask;
    byte |= data;
    return mpu6050_iic_write_byte(reg, &byte);
   }else{
		 return HAL_ERROR;
	 }
}

uint8_t mpu6050_iic_write_bit(uint8_t reg, uint8_t bit_num, uint8_t data){
	 uint8_t byte = 0;
   mpu6050_iic_read_byte(reg, &byte);
   byte = (data != 0) ? (byte | (1 << bit_num)) : (byte & ~(1 << bit_num));
   return mpu6050_iic_write_byte(reg, &byte);
}

//仅在inv_mpu.c使用  注意addr<<1
int my_i2c_write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data){
 return HAL_I2C_Mem_Write(&hi2c1, addr<<1, reg, I2C_MEMADD_SIZE_8BIT, data, len, MPU6050_IIC_MAX_DELAY);
} 
//仅在inv_mpu.c使用  注意addr<<1
int my_i2c_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf){
	 return  HAL_I2C_Mem_Read(&hi2c1, addr<<1, reg, I2C_MEMADD_SIZE_8BIT, buf, len, MPU6050_IIC_MAX_DELAY);
}