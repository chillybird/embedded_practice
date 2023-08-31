//
// Created by zhang on 2023/8/13.
//

#include "system_data.h"

struct rt_ringbuffer mess_buffer;
uint8_t mess_buff_data[MESS_BUF_SIZE];

void system_data_Init()
{
    rt_ringbuffer_init(&mess_buffer, mess_buff_data, MESS_BUF_SIZE);
}
