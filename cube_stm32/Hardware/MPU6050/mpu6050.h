//
// Created by zhang on 2023/8/19.
//

#ifndef CUBE_STM32_MPU6050_H
#define CUBE_STM32_MPU6050_H

#include "i2c.h"
#include "mpu6050_registers.h"

uint8_t MPU6050ReadID(void);
void MPU6050_WriteReg(uint8_t reg_addr, uint8_t reg_data);
void MPU6050_ReadData(uint8_t reg_addr, unsigned char* reg_data, uint8_t num);
void MPU6050_Init(void);

void MPU6050ReadTemp(short *tempData);
void MPU6050ReadGyro(short *gyroData);
void MPU6050ReadAcc(short *accData);
void MPU6050_ReturnTemp(float*Temperature);

#endif //CUBE_STM32_MPU6050_H
