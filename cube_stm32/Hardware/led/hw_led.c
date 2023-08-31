//
// Created by zhang on 2023/8/10.
//

#include "hw_led.h"

void LED_Init()
{
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}

void LED_Control(uint8_t r_val, uint8_t g_val, uint8_t b_val)
{
    if (r_val < 0 || r_val > 100 || g_val < 0 || g_val > 100 || b_val < 0 || b_val > 100 )
        return;
    __HAL_TIM_SetCompare(&htim3, LED_G_Pin, g_val);
    __HAL_TIM_SetCompare(&htim3, LED_B_Pin, b_val);
    __HAL_TIM_SetCompare(&htim3, LED_R_Pin, r_val);
}


