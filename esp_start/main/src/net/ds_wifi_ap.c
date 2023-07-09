#include <string.h>

#include "esp_wifi.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_event.h"
#include "esp_log.h"

#include "ds_wifi_ap.h"

// WiFi access point configuration from project configuration menu.
#define ESP_WIFI_SSID      CONFIG_ESP_AP_WIFI_SSID
#define ESP_WIFI_PASS      CONFIG_ESP_AP_WIFI_PASSWORD
#define WIFI_CHANNEL   CONFIG_ESP_AP_WIFI_CHANNEL
#define MAX_STA_CONN       CONFIG_ESP_AP_MAX_STA_CONN

static const char *TAG = "wifi softAP";

static uint8_t start_status = 0;

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d", MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_SSID,
            .ssid_len = strlen(ESP_WIFI_SSID),
            .channel = WIFI_CHANNEL,
            .password = ESP_WIFI_PASS,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d", ESP_WIFI_SSID, ESP_WIFI_PASS, WIFI_CHANNEL);
}

void ds_wifi_ap_start(void)
{
    if(start_status == 0){
        start_status = 1;
        ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
        wifi_init_softap();
    }else{
        ESP_LOGI(TAG, "ESP_WIFI_MODE_AP IS STARTING");
    }
}

void ds_wifi_ap_stop(){
    if(start_status == 1){
        start_status = 0;
        ESP_LOGI(TAG, "ESP_WIFI_MODE_AP STOP");
        ESP_ERROR_CHECK(esp_wifi_stop() );
        ESP_ERROR_CHECK(esp_wifi_deinit() );
    }else{
        ESP_LOGI(TAG, "ESP_WIFI_MODE_AP IS STOPING");
    }
}
