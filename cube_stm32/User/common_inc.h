//
// Created by zhang on 2023/8/11.
//

#ifndef CUBE_STM32_COMMON_INC_H
#define CUBE_STM32_COMMON_INC_H

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/

#include "main.h"
#include "tim.h"
#include "usart.h"

#include "Utils/uart_retarget.h"
#include "system_data.h"

#include "func_test.h"

void Main();

#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/
#endif


#endif //CUBE_STM32_COMMON_INC_H
