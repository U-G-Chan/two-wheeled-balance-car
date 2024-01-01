#include "mpu6050.h"
#include "mpu6050_iic.h"
#include "usart.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"

#include <math.h>

float Pitch,Roll,Yaw; 
short gyro[3], accel[3], sensors;

float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;

float Angle_Balance,Gyro_Balance;           //平衡倾角 平衡陀螺仪 转向陀螺仪

/*==============================基本设置==============================*/

//设置MPU6050的时钟源,见宏定义MPU6050_CLOCK_
uint8_t mpu6050_setClockSource(uint8_t source){
    return mpu6050_iic_write_bits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

//设置MPU6050的角速度量程，见宏定义MPU6050_GYRO_FS_
uint8_t mpu6050_setFullScaleGyroRange(uint8_t range) {
    return mpu6050_iic_write_bits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

//设置MPU6050的加速度量程，见宏定义MPU6050_ACCEL_FS_
uint8_t mpu6050_setFullScaleAccelRange(uint8_t range) {
   return mpu6050_iic_write_bits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

//设置MPU6050是否进入睡眠模式,enabled=1——睡觉  enabled=0——工作
uint8_t mpu6050_setSleepEnabled(uint8_t enabled) {
   return mpu6050_iic_write_bit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}
//读MPU6050器件地址
uint8_t mpu6050_ReadID(void)
{
	uint8_t ret = 0;
  mpu6050_iic_read_byte(MPU6050_RA_WHO_AM_I,&ret);    
	return ret;
}
//检测MPU6050 是否已经连接
uint8_t mpu6050_testConnection(void) {
   if(mpu6050_ReadID() == 0x68)  //0b01101000;
   return HAL_OK;
   	else return HAL_ERROR;
}

//设置 MPU6050 是否为AUX I2C线的主机
uint8_t mpu6050_setI2CMasterModeEnabled(uint8_t enabled) {
    return mpu6050_iic_write_bit(MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}
//设置MPU6050是否为AUX I2C线的主机
uint8_t mpu6050_setI2CBypassEnabled(uint8_t enabled) {
    return mpu6050_iic_write_bit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

//初始化MPU6050以进入可用状态
uint8_t mpu6050_init(void) {
	HAL_Delay(500);
	uint8_t ret = 0;
  ret += mpu6050_setClockSource(MPU6050_CLOCK_PLL_YGYRO); 		//设置时钟
  ret += mpu6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);	//陀螺仪最大量程 +-1000度每秒
  ret += mpu6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	//加速度度最大量程 +-2G
  ret += mpu6050_setSleepEnabled(0); 													//进入工作状态
	ret += mpu6050_setI2CMasterModeEnabled(0);									//不让MPU6050 控制AUXI2C
	ret += mpu6050_setI2CBypassEnabled(0);	 										//主控制器的I2C与	MPU6050的AUXI2C	直通。控制器可以直接访问HMC5883L
	return ret;
}

/*==============================基本设置==============================*/

/*==============================DMP==============================*/
//设置坐标系，根据MPU6050摆放方式修改
static signed char gyro_orientation[9] = {0, 1, 0,
                                          1, 0, 0,
                                          0, 0, 1};

static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;
    if (row[0] > 0) 		{ b = 0;}
    else if (row[0] < 0){ b = 4;}
    else if (row[1] > 0){ b = 1;}
    else if (row[1] < 0){ b = 5;}
    else if (row[2] > 0){ b = 2;}
    else if (row[2] < 0){ b = 6;}
    else								{ b = 7;}      // error
    return b;
}


static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;
    return scalar;
}

static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

		result = mpu_run_self_test(gyro, accel);
	if (result == 0x3) {//compass_self_test:0x4, accel_self_test:0x2, gyro_self_test:0x1
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
				message("[MPU6050]DMP setting bias succesfully ...\r\n");
    }
		
}



//MPU6050-DMP算法的初始化
uint8_t mpu6050_dmp_init(){
	if(mpu6050_testConnection() == HAL_OK){
		message("[MPU6050]Connect successfully.\r\n");
	}else{
		return HAL_ERROR;
	}	
	message("[MPU6050]DMP init ...\r\n");
	uint8_t ret = 0;
	ret += mpu_init();
	ret += mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
  ret += mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	ret += mpu_set_sample_rate(DEFAULT_MPU_HZ);
	ret += dmp_load_motion_driver_firmware();
	ret += dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
	ret += dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
	        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
	        DMP_FEATURE_GYRO_CAL);		
	ret += dmp_set_fifo_rate(DEFAULT_MPU_HZ);
	run_self_test();
	ret += mpu_set_dmp_state(1);
	return ret;
}

//读取MPU6050内置DMP的姿态信息
void mpu6050_read_dmp(void)
{	
	  unsigned long sensor_timestamp;
		unsigned char more;
		long quat[4];
		int ret = -1;
		if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more) != 0){
			return;
		}
		if (sensors & INV_WXYZ_QUAT )
		{    
			q0=quat[0] / q30;
			q1=quat[1] / q30;
			q2=quat[2] / q30;
			q3=quat[3] / q30;
			Pitch = asin(2*(q1*q3 - q0*q2))* 57.3; 	
			Roll  = atan2(2*(q0*q1 + q2*q3), 1 - 2*(q1*q1 - q2*q2))* 57.3;
			Yaw   = atan2(2*(q0*q3 + q1*q2), 1 - 2*(q2*q2 - q3*q3))* 57.3;
		}
}
void mpu6050_show_attitude(void){
	 //message("time=%d,Pitch=%4.2f,Roll=%4.2f,Yaw=%4.2f\r\n",HAL_GetTick(),Pitch,Roll,Yaw);
	message("time=%d,Pitch=%f\r\n",HAL_GetTick(),Pitch);
}

//获取角度
void mpu6050_get_angle(){
	mpu6050_read_dmp();//读取加速度、角速度、倾角
	Angle_Balance=Pitch;//更新平衡倾角
	Gyro_Balance=gyro[1];//更新平衡角速度
}

/*==============================DMP==============================*/
