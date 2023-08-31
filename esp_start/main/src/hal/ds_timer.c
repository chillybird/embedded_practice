#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#include "ds_timer.h"

#include "ds_system_data.h"
#include "ds_ft6336.h"

static const char* TAG = "timer";

static SemaphoreHandle_t xCountingSemaphore;

// 定时更新事件状态，触摸事件
void updateEvent(void)
{
    static TP_POSITION_T position;

    if (get_tp_action_status() > 0 )
    {
        count_tp_action_manage_time(); // 更新触摸的时间，用于判断长短按
        if(get_tp_action_status() <= 2) //检测开始触摸位置
        {
            get_ft6336_touch_sta(&position);
            if (position.status != 0) // 如果存在触摸状态
                set_tp_action_manage_start_point(position.x, position.y);

        } else if (get_tp_action_status() > 2)  //检测结束触摸位置
        {
            get_ft6336_touch_sta(&position);
            if(position.status != 0)
                set_tp_action_manage_stop_point(position.x, position.y);
        }
    }
}


static void second_periodic_timer_callback(void* arg)
{
    if (xSemaphoreGive(xCountingSemaphore) == pdFALSE)
        ESP_LOGI(TAG, "Give Clock Semaphore fail.");
}

static void event_periodic_timer_callback(void* arg)
{
    updateEvent();
}

static void vClockUpdateTask( void *pvParameters )
{
    const esp_timer_create_args_t sec_periodic_timer_args = {
            .callback = &second_periodic_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "second_period"
    };
    esp_timer_handle_t sec_periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&sec_periodic_timer_args, &sec_periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(sec_periodic_timer, 1000e3));
    ESP_LOGI(TAG, "Started clock update timer, time since boot: %lld us", esp_timer_get_time());

	for( ;; )
	{
		xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
        update_system_time_second();
	}
}

static void createEventTimer(void)
{
    const esp_timer_create_args_t event_periodic_timer_args = {
        .callback = &event_periodic_timer_callback,
        /* name is optional, but may help identify the timer when debugging */
        .name = "10ms_period"
    };
    esp_timer_handle_t event_periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&event_periodic_timer_args, &event_periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(event_periodic_timer, 10e3)); // 10ms
    ESP_LOGI(TAG, "Started event timer, time since boot: %lld us", esp_timer_get_time());
}

void ds_timer_init(void)
{
    UBaseType_t uxPriorityTimer = uxTaskPriorityGet(NULL);
    xCountingSemaphore = xSemaphoreCreateCounting(10, 0);

    createEventTimer();
    xTaskCreatePinnedToCore(vClockUpdateTask, "clock_task", 4096, NULL, uxPriorityTimer + 1, NULL, 0);
}

