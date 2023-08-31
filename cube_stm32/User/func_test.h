//
// Created by zhang on 2023/8/18.
//

#ifndef CUBE_STM32_FUNC_TEST_H
#define CUBE_STM32_FUNC_TEST_H

#include "common_inc.h"

#include "Hardware/led/hw_led.h"
#include "Hardware/key/hw_key.h"
#include "Hardware/AT24C02/at24c02.h"
#include "Hardware/DRV2605/drv2605.h"
#include "Hardware/MPU6050/mpu6050.h"

#define DATA_Size 256
#define EEP_Firstpage 0x00

void test_led();
void test_uart();
//void test_iwdg();
uint8_t test_eeprom();
void drv2605_Init();
void test_drv2605(uint8_t waveform);
void test_mpu6050();

#endif //CUBE_STM32_FUNC_TEST_H
