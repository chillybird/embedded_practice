#include "ds_ui_page_manage.h"
#include "ds_pwm.h"

static const char* TAG = "ds_ui_page_manage";

PAGE_MANAGE_T g_page_manage; // 记录当前页面
PAGE_HANDLE_T page_handles[PAGE_COUNT]; // 页面句柄数组
PAGE_TYPE_E page_default = PAGE_TYPE_TIME; // 默认显示页面
xQueueHandle g_ui_event_queue; // UI事件队列

bool init_flag = false; // 标记页面管理是否已经初始化

static void ds_ui_page_show(PAGE_TYPE_E page);


PAGE_TYPE_E ds_ui_get_now_show_page(void)
{
    return g_page_manage.now_show_page;
}

// 响应不同的触摸事件
void ds_ui_page_manage_send_event(TP_ACTION_E key,uint8_t touch_x,uint8_t touch_y)
{
    UI_EVENT_T event;
    event.key = key;
    event.touch_x = touch_x;
    event.touch_y = touch_y;

    if (key == TP_ACTION_LONG) // 长按进入设置页面
    {
        event.action = PAGE_TYPE_SETTING;
        xQueueSendFromISR(g_ui_event_queue, &event, NULL);
        // ESP_LOGI(TAG, "send event %d %d %d.\r\n", key, touch_x, touch_y);
        ESP_LOGI(TAG, "Switched to setting page.");
    }

    if (key == TP_ACTION_SHORT) // 响应页面点击事件
    {
        page_handles[g_page_manage.now_show_page].call_event(event); // 页面响应点击事件
        ESP_LOGI(TAG, "SHORT CLICK ACTION ON PAGE [%d].\r\n", g_page_manage.now_show_page);
    }
}

// 页面切换动作，来自其他页面
void ds_ui_page_manage_send_action(PAGE_TYPE_E action)
{
    UI_EVENT_T event;
    event.action = action;
    xQueueSendFromISR(g_ui_event_queue, &event, NULL);
    ESP_LOGI(TAG, "send action %d.\r\n", action);
}

static void uiPageManageTask(void *pvParameters)
{
    UI_EVENT_T event;
    for(;;)
    {
        xQueueReceive(g_ui_event_queue, &event, portMAX_DELAY);
        ESP_LOGI(TAG, "event %d %d %d %d.\r\n", event.key, event.touch_x, event.touch_y, event.action);
        // 调用每个页面的destory()函数销毁当前页面，并调用init()函数初始化新的页面
        ds_ui_page_show(event.action); // 显示新的页面
    }
}

void ds_ui_page_manage_init(void) 
{
    ds_ui_page_register(); // 注册所有页面

    ds_ui_page_show(page_default); // 显示默认页面
    g_ui_event_queue = xQueueCreate(10, sizeof(UI_EVENT_T)); // 创建UI事件队列

    xTaskCreate(uiPageManageTask, "uiPageManageTask", 4096, NULL, 5, NULL); // 创建页面管理任务，处理UI事件
}

// 将所有的Page Handle添加到数组中, 使用enum类型注册页面
void ds_ui_page_register(void)
{
    PAGE_HANDLE_T page_handle;

    // 注册菜单菜单页面
    ds_ui_registds_ui_mess_page(&page_handle);
    page_handles[page_handle.page_type] = page_handle;

    // 注册设置页面
    ds_ui_regist_setting_page(&page_handle);
    page_handles[page_handle.page_type] = page_handle;

    // 注册时钟页面
    ds_ui_regist_time_page(&page_handle);
    page_handles[page_handle.page_type] = page_handle;

    // 注册番茄时钟页面
    ds_ui_regist_tomato_page(&page_handle);
    page_handles[page_handle.page_type] = page_handle;
}

static void ds_ui_page_show(PAGE_TYPE_E page)
{
    if (page == page_default && !init_flag)
    {
        // 打开默认页面
        page_handles[page].page_init();
        g_page_manage.now_show_page = page_default;
        init_flag = true;
        ESP_LOGI(TAG, "init page manage with page [%d]\r\n", page_default);
    }
    else
    {
        // 跳转其他页面
        page_handles[g_page_manage.now_show_page].page_destory();
        page_handles[page].page_init();
        g_page_manage.now_show_page = page;
    }
}