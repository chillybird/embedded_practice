#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "ds_gpio.h"

#define DMA_CHAN 2

#define PIN_NUM_MISO 33
#define PIN_NUM_MOSI 26
#define PIN_NUM_CLK 25
#define PIN_NUM_CS 27

// To speed up transfers, every SPI transfer sends a bunch of lines. This define specifies how many. More means more memory use,
// but less overhead for setting up / finishing transfers. Make sure 240 is dividable by this.
#define PARALLEL_LINES 16

spi_device_handle_t spi;

void spi_send_cmd(const uint8_t cmd)
{
    esp_err_t ret;
    spi_transaction_t t;
    ds_gpio_set_screen_dc(0);
    ds_gpio_set_screen_cs(0);
    memset(&t, 0, sizeof(t)); // Zero out the transaction
    // t.flags=SPI_TRANS_USE_TXDATA;
    t.length = 8;                               // Command is 8 bits
    t.tx_buffer = &cmd;                         // The data is the cmd itself
    t.user = (void *)0;                         // D/C needs to be set to 0
    ret = spi_device_polling_transmit(spi, &t); // Transmit!
    ds_gpio_set_screen_cs(1);
    assert(ret == ESP_OK); // Should have had no issues.
}

void spi_send_data(const uint8_t data)
{
    esp_err_t ret;
    spi_transaction_t t;
    ds_gpio_set_screen_dc(1);
    ds_gpio_set_screen_cs(0);
    memset(&t, 0, sizeof(t));                   // Zero out the transaction
    t.length = 8;                               // Len is in bytes, transaction length is in bits.
    t.tx_buffer = &data;                        // Data
    t.user = (void *)1;                         // D/C needs to be set to 1
    ret = spi_device_polling_transmit(spi, &t); // Transmit!
    ds_gpio_set_screen_cs(1);
    assert(ret == ESP_OK); // Should have had no issues.
}

// This function is called (in irq context!) just before a transmission starts. It will
// set the D/C line to the value indicated in the user field.
void spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int)t->user;
    printf("dc callback\n");
    ds_gpio_set_screen_dc(dc);
}

void screen_spi_init(void)
{
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO, // MISO信号线
        .mosi_io_num = PIN_NUM_MOSI, // MOSI信号线
        .sclk_io_num = PIN_NUM_CLK,  // SCLK信号线
        .quadwp_io_num = -1,         // WP信号线，专用于QSPI的D2
        .quadhd_io_num = -1,         // HD信号线，专用于QSPI的D3
        .max_transfer_sz = 64 * 8,   // 最大传输数据大小
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 15 * 1000 * 1000, // Clock out at 26 MHz
        .mode = 0,                          // SPI mode 0
        .queue_size = 7,                    // We want to be able to queue 7 transactions at a time
        // .pre_cb=spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
    };

    /**
     * HSPI_HOST 是一个宏定义，用于标识 SPI 总线的主机。
     * 在 ESP32 系列芯片中，有两个硬件 SPI 总线可用，分别是 HSPI（Hardware SPI）和 VSPI（Volatile SPI）。
     * 
     * 物理引脚：HSPI 和 VSPI 使用不同的物理引脚进行通信。
     * HSPI 默认使用引脚 GPIO12 作为 MOSI（Master Out, Slave In）、GPIO13 作为 MISO（Master In, Slave Out）和 GPIO14 作为 SCLK（Serial Clock），
     * 而 VSPI 则使用 GPIO23、GPIO19 和 GPIO18。
     * 
     * 容量：HSPI 和 VSPI 总线可以连接不同数量的设备。
     * HSPI 总线可以连接多达 3 个设备，并具有 4 个片选引脚，
     * 而 VSPI 总线可以连接多达 3 个设备，并具有 3 个片选引脚。
     * 
     * 电气特性：HSPI 和 VSPI 总线具有不同的电气特性。
     * HSPI 总线支持电气特性的配置，例如输入输出电平（3.3V 或 1.8V）和推挽或开漏输出模式。
     * VSPI 总线则默认使用 3.3V 输入输出电平和推挽输出模式。
     * 
     * 可编程性：HSPI 和 VSPI 总线可以通过相应的寄存器进行配置和控制。
     * 对于 HSPI 总线，可以使用 SPI 控制寄存器来设置时钟速度、模式等参数。
     * 对于 VSPI 总线，可以使用 SPI 控制寄存器来设置时钟速度、模式等参数。
    */

    // Initialize the SPI bus
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, 0);
    ESP_ERROR_CHECK(ret);

    // Attach the LCD to the SPI bus
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
}

void screen_spi_test()
{
    spi_send_cmd(0x55);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    spi_send_data(0x55);
}