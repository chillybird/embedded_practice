//
// Created by zhang on 2023/8/11.
//

#include "common_inc.h"

void Main() {

    RetargetInit(&huart1); // USART printf重定向
    system_data_Init();
    LED_Init();

    Usart_SendString((uint8_t*)"hello stm32.\n");

//    drv2605_Init();
    test_mpu6050();

    while (true) {
        test_uart();
    }
}

