#include "ds_ui_settingpage.h"

static const char* TAG = "settingpage";

TaskHandle_t xTaskSettingpageHandle = NULL;


void ds_ui_regist_setting_page(PAGE_HANDLE_T* handle)
{
    handle->page_init = ds_ui_setting_page_init;
    handle->page_destory = ds_ui_setting_page_destory;
    handle->call_event = ds_ui_setting_page_callEvent;
    handle->page_type = PAGE_TYPE_SETTING;
}


static void vPageUpdateTask(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void ds_ui_setting_page_callEvent(UI_EVENT_T event)
{
    printf("setting page call event\n");
}

void ds_ui_setting_page_init()
{
    xTaskCreatePinnedToCore(vPageUpdateTask, "page update task", 4096, NULL, 10, &xTaskSettingpageHandle, tskNO_AFFINITY);
}

void ds_ui_setting_page_destory()
{
    if(xTaskSettingpageHandle != NULL)
        vTaskDelete(xTaskSettingpageHandle);
    else
        ESP_LOGE(TAG, "task settingpage is not created!");
}