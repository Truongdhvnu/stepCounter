#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f1xx_hal.h"

//------------MPU ADDRESS-----------------
#define MPU6050_ADDR 0xD0
//------------MPU sensitivity-------------
#define MPU6050_SENS 16384.0
//------------Define Register-------------
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define SMPLRT_DIV 0x19
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

//------------Define MPU6050 sensor-------
typedef struct
{
	I2C_HandleTypeDef* I2C;
} MPU6050_name;
//------------Define functions------------

void MPU6050_init(MPU6050_name* mpu, I2C_HandleTypeDef* hi2c_MPU, uint8_t sample_rate_devide);
uint16_t MPU6050_read_accX(MPU6050_name* mpu);
uint16_t MPU6050_read_accY(MPU6050_name* mpu);
uint16_t MPU6050_read_accZ(MPU6050_name* mpu);
#endif
