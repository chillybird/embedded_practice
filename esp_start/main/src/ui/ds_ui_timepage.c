#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ds_data_num.h"
#include "ds_screen.h"

#include "esp_log.h"

#include "ds_system_data.h"
#include "ds_ui_timepage.h"

static const char* TAG = "time_page";

typedef struct 
{
    uint8_t hour;
    uint8_t minute;
    uint8_t last_hour;
} TIME_PAGE_T;

TIME_PAGE_T g_time_page;

TaskHandle_t xTaskTimepageHandle = NULL;

// 更新时间函数
void ds_ui_timepage_updatetime(void)
{
    uint8_t now_index;

    g_time_page.hour = get_system_data().hour;
    g_time_page.minute = get_system_data().minute;

    now_index = g_time_page.hour / 10;
    EPD_Dis_Part(24, 167,hour0_bgImg[now_index],32,32); 
    
    now_index = g_time_page.hour % 10;
    EPD_Dis_Part(56, 167,hour1_bgImg[now_index],32,32); 

    EPD_Dis_Part(88, 167, colon_bg_10Img,32,16); 

    now_index = g_time_page.minute / 10;
    EPD_Dis_Part(104, 167,min0_bgImg[now_index],32,32); 
    
    now_index = g_time_page.minute % 10;
    EPD_Dis_Part(136, 167,min1_bgImg[now_index],32,32); 

    EPD_Part_Update();
    EPD_DeepSleep();

    g_time_page.last_hour = g_time_page.hour;
}

void draw_timepage_bg(void)
{
    // 绘制背景
    EPD_HW_Init();
	// EPD_SetRAMValue_BaseMap(NULL);
	EPD_SetRAMValue_BaseMap(num_bg);
	EPD_DeepSleep();
}

static void vtimeUpdateTask(void* arg)
{
    for (;;)
    {
        if (get_system_data().minute != g_time_page.minute)
        {
            ds_ui_timepage_updatetime();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void ds_ui_timepage_init(void)
{
    // 初始化时间显示页面
    memset(&g_time_page, 0, sizeof(TIME_PAGE_T));
    g_time_page.hour = 0;
    g_time_page.minute = 0;

    draw_timepage_bg();

    // 首次获取系统时间
    ds_ui_timepage_updatetime();

    // 创建时间更新任务
    xTaskCreatePinnedToCore(vtimeUpdateTask, "update time task", 4096, NULL, 10, &xTaskTimepageHandle, tskNO_AFFINITY);
}

void ds_ui_timepage_deinit(void)
{
    if (xTaskTimepageHandle)
        vTaskDelete(xTaskTimepageHandle);
    else
        ESP_LOGI(TAG,"Timepage is not created.\n");
}