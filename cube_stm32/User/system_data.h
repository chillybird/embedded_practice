//
// Created by zhang on 2023/8/13.
//

#ifndef CUBE_STM32_SYSTEM_DATA_H
#define CUBE_STM32_SYSTEM_DATA_H

#include "Middlewares/RingBuffer/ringbuffer.h"

#define MESS_BUF_SIZE 256
extern struct rt_ringbuffer mess_buffer;
extern uint8_t mess_buff_data[MESS_BUF_SIZE];

typedef enum
{
    MSG_NULL = 0,
    MSG_KEY1_PRESS,
    MSG_KEY2_PRESS,
    MSG_STR
} MSG_TYPE;

typedef struct
{
    MSG_TYPE msg_type;
    uint8_t msg_len;
} UART_MESS;

void system_data_Init();

#endif //CUBE_STM32_SYSTEM_DATA_H
