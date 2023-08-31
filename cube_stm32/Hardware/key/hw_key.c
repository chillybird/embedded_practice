//
// Created by zhang on 2023/8/10.
//

#include "hw_key.h"

void Key_Call(uint16_t GPIO_Pin)
{
    uint8_t key_num = 0;

    HAL_Delay(20);
    if (GPIO_Pin == Key_1_Pin && HAL_GPIO_ReadPin(Key_1_GPIO_Port, Key_1_Pin) == KEY_ON)
    {
        key_num = 1;
    }

    if (GPIO_Pin == Key_2_Pin && HAL_GPIO_ReadPin(Key_2_GPIO_Port, Key_2_Pin) == KEY_ON)
    {
        key_num = 2;
    }

    printf("key %d pressed.\n", key_num);
}