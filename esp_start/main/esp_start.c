#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include "driver/gpio.h"
#include "esp_sleep.h"

#include "esp_event.h"
#include "esp_wifi.h"

#include "ds_system_data.h"

#include "ds_uart.h"
#include "ds_timer.h"
#include "ds_spiffs.h"
#include "ds_nvs.h"
#include "ds_gpio.h"
#include "ds_pwm.h"

#include "ds_ft6336.h"
#include "ds_screen.h"

// ui
#include "ds_ui_page_manage.h"

// wifi
#include "ds_wifi_ap_sta.h"
#include "ds_tests.h"

#include "ds_http_request.h"

// font
#include "ds_font.h"

#define BUTTON_GPIO_NUM_DEFAULT  4
#define TWDT_TIMEOUT_S          3

void ds_system_init(void)
{
    ds_system_data_init(); // 系统数据初始化

    ds_nvs_init(); // nvs初始化

    ESP_ERROR_CHECK(esp_netif_init()); // 初始化网络接口库
    ESP_ERROR_CHECK(esp_event_loop_create_default()); // 创建默认的事件循环, 注册事件处理程序并响应各种事件，例如 Wi-Fi 连接、IP 获取等
}

static void background_task(void* arg)
{
    // 如果有http请求就一直保持AP&STA开启，否则在开启10min后关闭
    
    int apsta_close_count = 0;
    for(;;){
        // printf("background_task run... \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        //首次更新
        if(has_first_time_httpdata_request() == true && get_wifi_sta_status() == WIFI_STA_MODE_CONNECT_SUCCESS){
            ds_http_request_all();
            set_has_first_time_httpdata_request();
        }

        //下拉请求更新
        if(has_update_httpdata_request() == true){
            if(get_is_ap_sta_open()){
                if(get_wifi_sta_status() == WIFI_STA_MODE_CONNECT_SUCCESS){
                    ds_http_request_all();
                    set_update_httpdata_request(false);
                }
            }else{
                ds_wifi_send_event(AP_STA_START);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }

        //关闭wifi AP&STA 计时
        if(get_is_ap_sta_open() == true && apsta_close_count == 0){
            //重置10min计时
            apsta_close_count = (600 - 10);
        }

        if(apsta_close_count > 0){
            apsta_close_count --;
            if(apsta_close_count == 0){
                ds_wifi_send_event(AP_STA_STOP);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                set_update_httpdata_request(false);
            }
        }

        //设置页面 开启AP&STA
        // if(ds_ui_get_now_show_page() == PAGE_TYPE_SETTING){
        //     if(get_is_ap_sta_open() == false){
        //         ds_wifi_send_event(AP_STA_START);
        //         vTaskDelay(1000 / portTICK_PERIOD_MS);
        //     }
        // }
    }
}

static void sleep_mode_init(void)
{
    for(;;) 
    {
        set_tp_wackup_timeleft(600);
        
        //ap&sta关闭、且当前在主页、且超过5min未触摸时才进入低功耗
        do {
            esp_task_wdt_reset(); 
            vTaskDelay(pdMS_TO_TICKS(1000));
            count_tp_wackup_timeleft();
            printf("wait enter sleep mode run... %d\n",get_tp_wackup_timeleft());
            if(get_is_ap_sta_open() == false && ds_ui_get_now_show_page() == PAGE_TYPE_MEMU && get_tp_wackup_timeleft() == 0){
                break;
            }
        } while (1);

        ds_touch_gpio_isr_remove();
        gpio_wakeup_enable(BUTTON_GPIO_NUM_DEFAULT,GPIO_INTR_LOW_LEVEL);
        /* Wake up in 60 seconds, or when button is pressed */
        // esp_sleep_enable_timer_wakeup(60000000);
        esp_sleep_enable_gpio_wakeup();

        printf("Entering light sleep\n");

        /* Get timestamp before entering sleep */
        int64_t t_before_us = esp_timer_get_time();

        /* Enter sleep mode */
        esp_light_sleep_start();
        /* Execution continues here after wakeup */

        /* Get timestamp after waking up from sleep */
        int64_t t_after_us = esp_timer_get_time();

        /* Determine wake up reason */
        const char* wakeup_reason;
        uint32_t wackup_timeleft = 600;
        switch (esp_sleep_get_wakeup_cause()) {
            case ESP_SLEEP_WAKEUP_TIMER:
                wakeup_reason = "timer";
                wackup_timeleft = 5;
                break;
            case ESP_SLEEP_WAKEUP_GPIO:
                wakeup_reason = "pin";
                break;
            default:
                wakeup_reason = "other";
                break;
        }
        printf("Returned from light sleep, reason: %s, t=%lld ms, slept for %lld ms\n",
                wakeup_reason, t_after_us / 1000, (t_after_us - t_before_us) / 1000);

        set_tp_wackup_timeleft(wackup_timeleft); // 重新设置进入睡眠模式的剩余时间

        // gpio_wakeup_disable(BUTTON_GPIO_NUM_DEFAULT);
        // esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
        
        ds_touch_gpio_init();
        ds_touch_gpio_isr_add();
        reset_tp_action_manage();
    }
}

void app_main()
{
    ds_system_init();

    /* gpio */
    ds_gpio_init();

    /* beep */
    ds_pwm_init();

    /* http */ 
    ds_http_request_init(); //http请求初始化

    /* wifi ap sta*/
    ds_wifi_ap_sta_init(); // 初始化wifi的ap模式和sta模式
    ds_wifi_send_event(AP_STA_START);
    xTaskCreate(background_task, "background_task", 4096, NULL, 10, NULL);

    /*UART*/
    UART_Init();

    /*timer*/
    ds_timer_init(); // 创建定时器，每秒更新系统时间

    /* TP */
    init_ft6336(); 
    // ds_test_tp();

    /* screen */
    init_screen_interface();

    /* ui */
    ds_ui_page_manage_init();
    
    //进入低功耗模式
    // sleep_mode_init();

    // ds_test_adc(); // 测试adc

    // 测试字体库
    test_ds_font();

    for (;;)
    {
        esp_task_wdt_reset(); 
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    esp_task_wdt_delete(NULL);
}
