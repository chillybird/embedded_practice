#include "ds_ui_timepage.h"
#include "ds_data_num.h"

static const char* TAG = "timepage";

typedef struct 
{
    uint8_t hour;
    uint8_t minute;
    uint8_t last_hour;
} TIME_PAGE_T;

TIME_PAGE_T g_time_page;

TaskHandle_t xTaskTimepageHandle = NULL;

void ds_ui_regist_time_page(PAGE_HANDLE_T* handle)
{
    handle->page_init = ds_ui_timepage_init;
    handle->page_destory = ds_ui_timepage_destory;
    handle->call_event = ds_ui_time_page_callEvent;
    handle->page_type = PAGE_TYPE_TIME;
}

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

void ds_ui_time_page_callEvent(UI_EVENT_T event)
{
    printf("time page call event\n");
    ds_ui_page_manage_send_action(PAGE_TYPE_MEMU); // 返回时钟页面
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

void ds_ui_timepage_destory(void)
{
    if (xTaskTimepageHandle)
        vTaskDelete(xTaskTimepageHandle);
    else
        ESP_LOGI(TAG,"Timepage is not created!\n");
}