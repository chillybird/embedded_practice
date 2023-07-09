#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#include "ds_timer.h"

#include "ds_system_data.h"

static const char* TAG = "timer";

static SemaphoreHandle_t xCountingSemaphore;

static void second_periodic_timer_callback(void* arg)
{
    if (xSemaphoreGive(xCountingSemaphore) == pdFALSE)
        ESP_LOGI(TAG, "Give Semaphore fail.");
}

static void vTimerTask( void *pvParameters )
{
    const esp_timer_create_args_t sec_periodic_timer_args = {
            .callback = &second_periodic_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "second_period"
    };
    esp_timer_handle_t sec_periodic_timer;
    
    ESP_LOGI(TAG, "Started timers, time since boot: %lld us", esp_timer_get_time());
    ESP_ERROR_CHECK(esp_timer_create(&sec_periodic_timer_args, &sec_periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(sec_periodic_timer, 1000e3));

	for( ;; )
	{
		xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
        update_system_time_second();
	}

}

void ds_timer_init(void)
{
    UBaseType_t uxPriorityTimer = uxTaskPriorityGet(NULL);
    xCountingSemaphore = xSemaphoreCreateCounting(10, 0);
    
    xTaskCreatePinnedToCore(vTimerTask, "timer_task", 4096, NULL, uxPriorityTimer + 1, NULL, 0);
}

