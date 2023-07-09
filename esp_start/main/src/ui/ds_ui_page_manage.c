#include "esp_log.h"

#include "ds_system_data.h"
#include "ds_ui_page_manage.h"

#define TAG "UI page manage"

PAGE_TYPE_E ds_ui_get_now_show_page(void)
{
    ESP_LOGI(TAG, "show page.\r\n");
    return PAGE_TYPE_MEMU;
}


void ds_ui_page_manage_send_event(TP_ACTION_E key,uint8_t touch_x,uint8_t touch_y)
{
    ESP_LOGI(TAG, "send event %d %d %d.\r\n", key, touch_x, touch_y);
}


void ds_ui_page_manage_send_action(PAGE_TYPE_E action)
{
    ESP_LOGI(TAG, "send action %d.\r\n", action);
}


void ds_ui_page_manage_init(void) 
{

}

