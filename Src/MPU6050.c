#include "MPU6050.h"

void MPU6050_init(MPU6050_name* mpu, I2C_HandleTypeDef* hi2c_MPU, uint8_t sample_rate_devide) {
		mpu->I2C = hi2c_MPU;
		uint8_t data = 0;
		// check if sensor is responding
		HAL_I2C_Mem_Read(hi2c_MPU, MPU6050_ADDR, WHO_AM_I, 1, &data, 1, 1000);
		if(data != 0x68) return;
		// wake the sensor up
		data = 0x00;
		HAL_I2C_Mem_Write(hi2c_MPU, MPU6050_ADDR, PWR_MGMT_1, 1,&data, 1, 1000);
		// set the sample rate = 8k/(1 + sample_rate_devide)
		data = sample_rate_devide;
		HAL_I2C_Mem_Write(hi2c_MPU, MPU6050_ADDR, SMPLRT_DIV, 1, &data, 1, 1000);
		// configure the Accelerometer and Gyroscope
		data = 0x00;
		HAL_I2C_Mem_Write(hi2c_MPU, MPU6050_ADDR, GYRO_CONFIG, 1, &data, 1, 1000);
		HAL_I2C_Mem_Write(hi2c_MPU, MPU6050_ADDR, ACCEL_CONFIG, 1, &data, 1, 1000);
}

uint16_t MPU6050_read_accX(MPU6050_name* mpu) {
		uint8_t data[2];
		HAL_I2C_Mem_Read(mpu->I2C, MPU6050_ADDR, ACCEL_XOUT_H, 1, data, 2, 1000);
		return (int16_t)(data[0] << 8 | data[0]);
}

uint16_t MPU6050_read_accY(MPU6050_name* mpu) {
		uint8_t data[2];
		HAL_I2C_Mem_Read(mpu->I2C, MPU6050_ADDR, ACCEL_YOUT_H, 1, data, 2, 1000);
		return (int16_t)(data[0] << 8 | data[0]);
}	


uint16_t MPU6050_read_accZ(MPU6050_name* mpu) {
		uint8_t data[2];
		HAL_I2C_Mem_Read(mpu->I2C, MPU6050_ADDR, ACCEL_ZOUT_H, 1, data, 2, 1000);
		return (int16_t)(data[0] << 8 | data[0]);
}
