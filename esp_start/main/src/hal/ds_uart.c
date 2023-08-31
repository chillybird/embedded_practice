#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ds_uart.h"
#include "ds_ui_messpage.h"

static const char *TAG = "board_uart";

static QueueHandle_t s_uart0Queue;

void UART_Init(void)
{
    // Configure parameters of an UART driver,
    // communication pins and install the driver
    uart_config_t uart_config = {
            .baud_rate = 115200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
        };

    uart_param_config(UART_NUM_0, &uart_config);                                                              // 配置串口0参数
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // 配置串口0引脚
    // Install UART driver, and get the queue.
    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, BUF_SIZE * 2, 100, &s_uart0Queue, 0); // 安装UART驱动程序

    // Create a task to handler UART event from ISR
    xTaskCreate(uartEventTask, "uartEventTask", 2048, NULL, 12, NULL);
}

void uartEventTask(void *pvParameters)
{
    // const char* hello = "hello\r\n";
    // uart_write_bytes(UART_NUM_0, hello, strlen(hello));

    uart_event_t event;
    uint8_t *pTempBuf = (uint8_t *)malloc(UART_MAX_NUM_RX_BYTES);

    for (;;)
    {
        // Waiting for UART event.
        if (xQueueReceive(s_uart0Queue, (void *)&event, (portTickType)portMAX_DELAY))
        {
            bzero(pTempBuf, UART_MAX_NUM_RX_BYTES);

            switch (event.type)
            {
            // Event of UART receving data
            // We'd better handler data event fast, there would be much more data events than
            // other types of events. If we take too much time on data event, the queue might be full.
            case UART_DATA:
                ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                // uart_read_bytes(UART_NUM_0, pTempBuf, event.size, portMAX_DELAY); // event.size
                uart_read_bytes(UART_NUM_0, pTempBuf, BUF_SIZE * 2, pdMS_TO_TICKS(100)); 
                uart_write_bytes(UART_NUM_0, (const char *)pTempBuf, event.size);
                ds_ui_show_mess((char*)pTempBuf);
                break;

            // Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGI(TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(UART_NUM_0);
                xQueueReset(s_uart0Queue);
                break;

            // Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGI(TAG, "ring buffer full");
                // If buffer full happened, you should consider encreasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(UART_NUM_0);
                xQueueReset(s_uart0Queue);
                break;

            case UART_PARITY_ERR:
                ESP_LOGI(TAG, "uart parity error");
                break;

            // Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGI(TAG, "uart frame error");
                break;

            // Others
            default:
                ESP_LOGI(TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }

    free(pTempBuf);
    pTempBuf = NULL;
    vTaskDelete(NULL);
}

char *pack_touch_pos_data(uint16_t pos[], uint8_t size, uint8_t *length)
{
    if (size == 4)
    {
        // 计算拼接后字符串的长度
        *length = snprintf(NULL, 0, "@%d_%d_%d_%d#", pos[0], pos[1], pos[2], pos[3]);
        // 为拼接后的字符串分配空间
        char *result = (char *)malloc((*length + 1) * sizeof(char));
        // 将字符串和整数拼接到一起
        sprintf(result, "@%d_%d_%d_%d#", pos[0], pos[1], pos[2], pos[3]);
        return result;
    }
    else
    {
        // 计算拼接后字符串的长度
        *length = snprintf(NULL, 0, "@%d_%d#", pos[0], pos[1]);
        // 为拼接后的字符串分配空间
        char *result = (char *)malloc((*length + 1) * sizeof(char));
        // 将字符串和整数拼接到一起
        sprintf(result, "@%d_%d#", pos[0], pos[1]);
        return result;
    }
}

void send_uart_touch_pos(touch_pos_t packet)
{
    static uint16_t pos_x = 0, pos_y = 0;
    if (pos_x == packet.pos[0] && pos_y == packet.pos[1])
        return;

    uint8_t length = 0;
    uint8_t *buf = (uint8_t *)pack_touch_pos_data(packet.pos, packet.size, &length);
    uart_write_bytes(UART_NUM_0, (const char *)buf, length);

    pos_x = packet.pos[0];
    pos_y = packet.pos[1];
    
    free(buf);
    buf = NULL;
}

void send_uart_double_touch_pos(double_touch_pos_t packet)
{
    static uint16_t pos_x_0 = 0, pos_y_0 = 0, pos_x_1 = 0, pos_y_1 = 0;
    if (pos_x_0 == packet.double_pos[0] && pos_y_0 == packet.double_pos[1] && pos_x_1 == packet.double_pos[2] && pos_y_1 == packet.double_pos[3])
        return;

    uint8_t length = 0;
    uint8_t *buf = (uint8_t *)pack_touch_pos_data(packet.double_pos, packet.size, &length);
    uart_write_bytes(UART_NUM_0, (const char *)buf, length);

    pos_x_0 = packet.double_pos[0];
    pos_y_0 = packet.double_pos[1];
    pos_x_1 = packet.double_pos[2];
    pos_y_1 = packet.double_pos[3];

    free(buf);
    buf = NULL;
}