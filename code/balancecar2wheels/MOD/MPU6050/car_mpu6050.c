#include "car_mpu6050.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <stdio.h>

static float pitch,roll,yaw; 		    //ŷ����
static short aacx,aacy,aacz;					//���ٶȴ�����ԭʼ����
static short gyrox,gyroy,gyroz;			//������ԭʼ����
static float temp;					    			//�¶�

//��ʼ��
uint8_t mpu6050_init(){
	int ret = 0;
	ret += MPU_Init();			 //MPU6050��ʼ��
  ret += mpu_dmp_init();	 //dmp��ʼ��
	return ret;
}

//��mpu6050�и��²���
uint8_t mpu6050_update_angle(){
	return mpu_dmp_get_data(&pitch, &roll, &yaw);	
}

uint8_t mpu6050_update_acc(){
	return MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
}

uint8_t mpu6050_update_gyr(){
	return MPU_Get_Gyroscope(&gyrox,&gyrox,&gyrox);
}

void mpu6050_show_angle(){
	printf("X:%.1f  Y:%.1f  Z:%.1f\r\n",roll,pitch,yaw);//����1����ɼ���Ϣ
}

void mpu6050_get_balnace_feedback(float *forward_angle, short *forward_gyr){
	*forward_angle = roll;
	*forward_gyr   = gyrox;
}

