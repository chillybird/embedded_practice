//
// Created by zhang on 2023/8/10.
//

#ifndef CUBE_STM32_HW_LED_H
#define CUBE_STM32_HW_LED_H

#include "main.h"
#include "tim.h"

#define LED_G_Pin TIM_CHANNEL_3 // PB0
#define LED_B_Pin TIM_CHANNEL_4 // PB1
#define LED_R_Pin TIM_CHANNEL_2 // PB5

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGBColor;

void LED_Init();
void LED_Control(uint8_t r_val, uint8_t g_val, uint8_t b_val);

#endif //CUBE_STM32_HW_LED_H
