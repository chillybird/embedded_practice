#include "esp_system.h"
#include "esp_spi_flash.h"

// hal
#include "ds_uart.h"
#include "ds_timer.h"
#include "ds_spiffs.h"
#include "ds_nvs.h"
#include "ds_system_data.h"
#include "ds_gpio.h"
#include "ds_pwm.h"
#include "ds_adc.h"

// driver
#include "ds_ft6336.h"
#include "ds_screen.h"

// ui
#include "ds_ui_page_manage.h"
#include "ds_ui_timepage.h"

// net
#include "ds_wifi_ap.h"
#include "ds_http_server.h"
#include "ds_wifi_sta.h"
#include "ds_wifi_ap_sta.h"
#include "ds_wifi_scan.h"

#include "ds_tests.h"

#ifdef CONFIG_IDF_TARGET_ESP32
#define CHIP_NAME "ESP32"
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S2BETA
#define CHIP_NAME "ESP32-S2 Beta"
#endif

TP_POSITION_T position;

void ds_test_sys_info(void)
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
            CHIP_NAME,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}

void ds_test_spiffs(void)
{
    /*SPIFFS*/
    init_spiffs();
    ds_spiffs_test();
    // ds_spiffs_listFiles();
    // ds_spiffs_deleteEntry("/spiffs/foo.txt");
    ds_spiffs_deinit();
}

void ds_test_gpio(void)
{
    /*gpio*/
    ds_gpio_init();
}

void ds_test_nvs(void)
{
    /*NVS*/
    char *ssid = "zhcode";
    char *pwd = "zhcode123";
    ds_nvs_init();
    ds_nvs_save_wifi_info(ssid, pwd);
    ds_nvs_read_wifi_info();
}

void ds_test_screen(void)
{
    /*水墨屏*/
    init_screen_interface();
    test_SSD1681();
}

static void vTPTestTask(void* arg)
{
    for(;;)
    {
        get_ft6336_touch_sta(&position);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void ds_test_tp(void)
{
    /*TP*/
    init_ft6336();
    xTaskCreatePinnedToCore(vTPTestTask, "TP test task", 4096, NULL, 10, NULL, tskNO_AFFINITY);
}

void ds_test_wifi_ap(void)
{
    /* wifi ap*/
    ds_wifi_ap_start();
    ds_http_server_init(); //http server
}

void ds_test_wifi_sta(void)
{
     char *ssid="zhcode";
     char *psw="zhcode123";
     ds_nvs_save_wifi_info(ssid,psw);
    /* wifi station */
    ds_wifi_sta_start();
}

void ds_test_beep(void)
{
    ds_pwm_init();
    send_beep_event(BEEP_SHORT_500MS);
}

void ds_test_wifi_scan(void)
{
    ds_wifi_scan_start();
}

void ds_test_adc(void)
{
    ds_batteryLevelMonitor_init();
    printf("test voltage: %d\r\n", getBatteryLevel());
}
