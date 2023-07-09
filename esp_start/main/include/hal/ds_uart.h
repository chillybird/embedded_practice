#ifndef __DS_UART_H__
#define __DS_UART_H__

#include "driver/uart.h"

#define BUF_SIZE (1024)
#define UART_MAX_NUM_RX_BYTES (1024)

typedef struct {
    int size;
    uint16_t pos[2];
} touch_pos_t;

typedef struct {
    int size;
    uint16_t double_pos[4];
} double_touch_pos_t;

void UART_Init(void);
void uartEventTask(void *pvParameters);
char* pack_touch_pos_data(uint16_t pos[], uint8_t size, uint8_t *length);
void send_uart_touch_pos(touch_pos_t packet);
void send_uart_double_touch_pos(double_touch_pos_t packet);

#endif // DS_UART_H__