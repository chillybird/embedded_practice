#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_http_client.h"
#include "cJSON.h"

#include "ds_system_data.h"
#include "ds_http_request.h"

static const char *TAG = "HTTP_CLIENT";

xQueueHandle http_request_event_queue;

static void cjson_time_info(char *text)
{
    cJSON *root, *psub;
    char time[20];
    // 截取有效json
    char *index = strchr(text, '{');
    strcpy(text, index);
    root = cJSON_Parse(text);
    if (root != NULL)
    {
        psub = cJSON_GetObjectItem(root, "sysTime1");
        sprintf(time, "%s", psub->valuestring);
        ESP_LOGI(TAG, "sysTime:%s", time);
    }
    cJSON_Delete(root);

    int len = strlen(time);
    if (len < 11)
    {
        return;
    }
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    hour = (time[8] - '0') * 10 + time[9] - '0';
    minute = (time[10] - '0') * 10 + time[11] - '0';
    second = (time[12] - '0') * 10 + time[13] - '0';
    update_system_time(hour, minute, second);
}

// 事件回调
static esp_err_t _http_time_event_handle(esp_http_client_event_t *evt)
{
    static char *output_buffer; // Buffer to store response of http request from event handler
    static int output_len;      // Stores number of bytes read
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA: // 接收数据事件
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            if (output_buffer == NULL)
            {
                output_buffer = (char *)malloc(esp_http_client_get_content_length(evt->client));
                output_len = 0;
                if (output_buffer == NULL)
                {
                    ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                    return ESP_FAIL;
                }
            }
            memcpy(output_buffer + output_len, evt->data, evt->data_len);
            printf("%.*s\n", output_len, output_buffer);
            cjson_time_info((char *)output_buffer);
            output_len += evt->data_len;
        }
        break;
    case HTTP_EVENT_ERROR:
        break;
    case HTTP_EVENT_ON_CONNECTED:
        break;
    case HTTP_EVENT_HEADERS_SENT:
        break;
    case HTTP_EVENT_ON_HEADER:
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
        if (output_buffer != NULL)
        {
            // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
            // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
            free(output_buffer);
            output_buffer = NULL;
        }
        output_len = 0;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
        if (err != 0)
        {
            ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }
        if (output_buffer != NULL)
        {
            free(output_buffer);
            output_buffer = NULL;
        }
        output_len = 0;
        break;
    }
    return ESP_OK;
}

void http_time_get()
{
    // http client配置
    esp_http_client_config_t config = {
        .method = HTTP_METHOD_GET, // get请求
        .url = "http://quan.suning.com/getSysTime.do",
        .event_handler = _http_time_event_handle, // 注册时间回调
        .skip_cert_common_name_check = true,
    };
    esp_http_client_handle_t time_client = esp_http_client_init(&config); // 初始化配置
    esp_err_t err = esp_http_client_perform(time_client);                 // 执行请求

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Status = %d, content_length = %d",
                 esp_http_client_get_status_code(time_client),     // 状态码
                 esp_http_client_get_content_length(time_client)); // 数据长度
    }
    esp_http_client_cleanup(time_client); // 断开并释放资源
}

void ds_http_request_type(HTTP_REQUEST_TYPE_E type)
{
    HTTP_REQUEST_TYPE_E evt;
    evt = type;
    xQueueSend(http_request_event_queue, &evt, 0);
}

void ds_http_request_all(){
    ds_http_request_type(HTTP_GET_TIME);
    // ds_http_request_type(HTTP_GET_WEATHER);
}

static void http_request_task(void *pvParameters)
{
    while (1)
    {
        HTTP_REQUEST_TYPE_E evt;
        xQueueReceive(http_request_event_queue, &evt, portMAX_DELAY);
        ESP_LOGI(TAG, "http_get_task %d", evt);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        switch (evt)
        {
        case HTTP_GET_TIME:
            http_time_get();
            break;
        // case HTTP_GET_WEATHER:
        //     http_weather_get();
        //     break;
        // case HTTP_GET_FANS:
        //     http_fans_get();
        //     break;
        // case HTTP_GET_CITY:
        //     http_city_get();
        //     break;
        default:
            break;
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void ds_http_request_init(void)
{
    http_request_event_queue = xQueueCreate(10, sizeof(HTTP_REQUEST_TYPE_E));
    xTaskCreate(&http_request_task, "http_request_task", 4096, NULL, 5, NULL);
}
