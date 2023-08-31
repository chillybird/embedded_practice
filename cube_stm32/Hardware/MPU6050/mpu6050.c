//
// Created by zhang on 2023/8/19.
//

#include "mpu6050.h"

void MPU6050_WriteReg(uint8_t reg_addr, uint8_t reg_data)
{
    I2C_WriteRegister(MPU6050_ADDRESS, reg_addr, 1, &reg_data);
}

void MPU6050_ReadData(uint8_t reg_addr, unsigned char* reg_data, uint8_t num)
{
    I2C_ReadRegister(MPU6050_ADDRESS, reg_addr, num, reg_data);
}

void MPU6050_Init(void)
{
    HAL_Delay(100); //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
    MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率
    MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //配置加速度传感器工作在16G模式
    MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
    HAL_Delay(100);
}

uint8_t MPU6050ReadID(void) {
    unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I, &Re, 1);    //读器件地址
    if (Re != 0x68) {
        I2C_DEBUG("MPU6050 dectected error!\r\n");
        return 0;
    } else {
        I2C_DEBUG("MPU6050 ID = %d\r\n", Re);
        return 1;
    }
}

/**
  * @brief   读取MPU6050的加速度数据
  * @param
  * @retval
  */
void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的角加速度数据
  * @param
  * @retval
  */
void MPU6050ReadGyro(short *gyroData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的原始温度数据
  * @param
  * @retval
  */
void MPU6050ReadTemp(short *tempData)
{
    uint8_t buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param
  * @retval
  */
void MPU6050_ReturnTemp(float *Temperature)
{
    short temp3;
    uint8_t buf[2];

    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H, buf,2);     //读取温度值
    temp3= (buf[0] << 8) | buf[1];
    *Temperature=((double) temp3/340.0)+36.53;

}
