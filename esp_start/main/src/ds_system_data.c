#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_log.h"

#include "ds_ui_page_manage.h"
#include "ds_system_data.h"

static const char *TAG = "ds_system_data";

SYSTRM_DATA_T g_system_data;

////////////////////SYSTEM DATA////////////////////////////////
void ds_system_data_init()
{
    memset(&g_system_data, 0, sizeof(SYSTRM_DATA_T));
    g_system_data.first_time_httpdata_request = true;
    g_system_data.update_httpdata_request = false;
    g_system_data.has_wifi_info = false;
    g_system_data.hour = 10;
    g_system_data.minute = 10;
}

SYSTRM_DATA_T get_system_data()
{
    return g_system_data;
}

//////////////////WIFI INFO MANAGE //////////////////////////////
void set_system_data_wifi_info(char *p_ssid, char *p_psw)
{
    int p_ssidlen = strlen(p_ssid);
    int p_pswlen = strlen(p_psw);
    if (p_pswlen >= MAX_SETTING_SSID_LEN || p_ssidlen >= MAX_SETTING_PSW_LEN)
    {
        ESP_LOGE(TAG, "MAX_SETTING_SSID_PWD_LEN ERROR");
    }
    g_system_data.has_wifi_info = true;
    g_system_data.setting_ssid_len = p_ssidlen;
    g_system_data.setting_psw_len = p_pswlen;
    memcpy(g_system_data.setting_ssid, p_ssid, p_ssidlen);
    memcpy(g_system_data.setting_psw, p_psw, p_pswlen);
    g_system_data.setting_ssid[p_ssidlen] = '\0';
    g_system_data.setting_psw[p_pswlen] = '\0';
}

void print_system_data_wifi_info()
{
    printf("\r\nwifi_ssid:");
    for (int i = 0; i < g_system_data.setting_ssid_len; i++)
    {
        printf("%c", g_system_data.setting_ssid[i]);
    }

    printf("\r\nwifi_password:");
    for (int i = 0; i < g_system_data.setting_psw_len; i++)
    {
        printf("%c", g_system_data.setting_psw[i]);
    }
    printf("\r\n");
}

bool has_wifi_info(void)
{
    return g_system_data.has_wifi_info;
}

//////////////////NET STATUS/////////////////////////////////////
// 获取sta模式状态
WIFI_STA_MODE_STATUS_E get_wifi_sta_status()
{
    return g_system_data.wifi_sta_status;
}

void set_wifi_sta_status(WIFI_STA_MODE_STATUS_E status)
{
    g_system_data.wifi_sta_status = status;
}

// 获取ap模式状态
WIFI_AP_MODE_STATUS_E get_wifi_ap_status()
{
    return g_system_data.wifi_ap_status;
}

void set_wifi_ap_status(WIFI_AP_MODE_STATUS_E status)
{
    g_system_data.wifi_ap_status = status;
}

void set_is_ap_sta_open(bool isopen)
{
    g_system_data.is_ap_sta_open = isopen;
}

bool get_is_ap_sta_open()
{
    return g_system_data.is_ap_sta_open;
}

////////////////HTTP//////////////////////
bool has_first_time_httpdata_request()
{
    return g_system_data.first_time_httpdata_request;
}

void set_has_first_time_httpdata_request()
{
    g_system_data.first_time_httpdata_request = false;
}

bool has_update_httpdata_request()
{
    return g_system_data.update_httpdata_request;
}

void set_update_httpdata_request(bool update)
{
    g_system_data.update_httpdata_request = update;
}

////////////////////TP ACTION MANAGE ////////////////////////////
void clear_tp_action()
{
    g_system_data.tp_action_manage.tp_action = TP_ACTION_NULL;
}

uint8_t get_tp_action_status()
{
    return g_system_data.tp_action_manage.status;
}

TP_ACTION_E get_tp_action()
{
    return g_system_data.tp_action_manage.tp_action;
}

void check_tp_action()
{
    g_system_data.tp_action_manage.status = 0;
    TP_ACTION_MANAGE_T *m_manage = &g_system_data.tp_action_manage;
    // if(abs(m_action_manage->tp_start_x - m_action_manage->tp_stop_x) > ){

    // }
    // ESP_LOGI(TAG,"timecount %d",m_manage->tp_interval_timecount);
    ESP_LOGI(TAG, "start %d %d stop %d %d", m_manage->tp_start_x, m_manage->tp_start_y, m_manage->tp_stop_x, m_manage->tp_stop_y);
    if (m_manage->tp_stop_x == 0 && m_manage->tp_stop_y == 0 && m_manage->tp_start_x != 0 && m_manage->tp_start_y != 0)
    {
        ESP_LOGI(TAG, "action TP_ACTION_SHORT");
        g_system_data.tp_action_manage.tp_action = TP_ACTION_SHORT;
        ds_ui_page_manage_send_event(g_system_data.tp_action_manage.tp_action, g_system_data.tp_action_manage.tp_start_x, g_system_data.tp_action_manage.tp_start_y);
        return;
    }
    if (abs(m_manage->tp_start_x - m_manage->tp_stop_x) > abs(m_manage->tp_start_y - m_manage->tp_stop_y))
    {
        // 左右滑动
        if (abs(m_manage->tp_start_x - m_manage->tp_stop_x) > 10)
        {
            if (m_manage->tp_start_x > m_manage->tp_stop_x)
            {
                ESP_LOGI(TAG, "action TP_ACTION_MOVE_LEFT");
                g_system_data.tp_action_manage.tp_action = TP_ACTION_MOVE_LEFT;
            }
            else
            {
                ESP_LOGI(TAG, "action TP_ACTION_MOVE_RIGHT");
                g_system_data.tp_action_manage.tp_action = TP_ACTION_MOVE_RIGHT;
            }
            ds_ui_page_manage_send_event(g_system_data.tp_action_manage.tp_action, g_system_data.tp_action_manage.tp_start_x, g_system_data.tp_action_manage.tp_start_y);
            return;
        }
    }
    else
    {
        // 上下滑动
        if (abs(m_manage->tp_start_y - m_manage->tp_stop_y) > 10)
        {
            if (m_manage->tp_start_y > m_manage->tp_stop_y)
            {
                ESP_LOGI(TAG, "action TP_ACTION_MOVE_UP");
                g_system_data.tp_action_manage.tp_action = TP_ACTION_MOVE_UP;
            }
            else
            {
                ESP_LOGI(TAG, "action TP_ACTION_MOVE_DOWN");
                g_system_data.tp_action_manage.tp_action = TP_ACTION_MOVE_DOWN;
            }
            ds_ui_page_manage_send_event(g_system_data.tp_action_manage.tp_action, g_system_data.tp_action_manage.tp_start_x, g_system_data.tp_action_manage.tp_start_y);
            return;
        }
    }

    // 300ms
    if (m_manage->tp_interval_timecount < 30)
    {
        if ((m_manage->tp_stop_x + m_manage->tp_stop_y + m_manage->tp_start_x + m_manage->tp_start_y) == 0)
            return;
        ESP_LOGI(TAG, "action TP_ACTION_SHORT");
        g_system_data.tp_action_manage.tp_action = TP_ACTION_SHORT;
        ds_ui_page_manage_send_event(g_system_data.tp_action_manage.tp_action, g_system_data.tp_action_manage.tp_start_x, g_system_data.tp_action_manage.tp_start_y);
        return;
    }
    // 1.5s
    if (m_manage->tp_interval_timecount > 150)
    {
        if (m_manage->tp_start_x != 0 && m_manage->tp_start_y != 0 && m_manage->tp_stop_x != 0 && m_manage->tp_stop_y != 0)
        {
            return;
        }
        ESP_LOGI(TAG, "action TP_ACTION_LONG");
        g_system_data.tp_action_manage.tp_action = TP_ACTION_LONG;
        ds_ui_page_manage_send_event(g_system_data.tp_action_manage.tp_action, g_system_data.tp_action_manage.tp_start_x, g_system_data.tp_action_manage.tp_start_y);
        return;
    }
}

void set_tp_action_manage_stop_point(uint16_t x, uint16_t y)
{
    g_system_data.tp_action_manage.tp_stop_x = x;
    g_system_data.tp_action_manage.tp_stop_y = y;
}

void set_tp_action_manage_start_point(uint16_t x, uint16_t y)
{
    g_system_data.tp_action_manage.status++;
    if (g_system_data.tp_action_manage.status >= 10)
    {
        g_system_data.tp_action_manage.status = 10;
    }
    g_system_data.tp_action_manage.tp_start_x = x;
    g_system_data.tp_action_manage.tp_start_y = y;
}

// 10ms once count
void count_tp_action_manage_time()
{
    g_system_data.tp_action_manage.tp_interval_timecount++;
    // 100s
    if (g_system_data.tp_action_manage.tp_interval_timecount > 10000)
    {
        g_system_data.tp_action_manage.tp_interval_timecount = 10000;
    }
}

void reset_tp_action_manage()
{
    memset(&g_system_data.tp_action_manage, 0, sizeof(TP_ACTION_MANAGE_T));
    // set check on
    g_system_data.tp_action_manage.status = 1;
}

void set_tp_wackup_timeleft(uint32_t timeleft)
{
    // 600/1s 10min
    g_system_data.tp_wackup_timeleft = timeleft;
}

void count_tp_wackup_timeleft()
{
    if (g_system_data.tp_wackup_timeleft > 0)
    {
        g_system_data.tp_wackup_timeleft--;
    }
}

uint32_t get_tp_wackup_timeleft()
{
    return g_system_data.tp_wackup_timeleft;
}

////////////////TIME/////////////////////
void update_system_time(uint8_t hour, uint8_t minute, uint8_t second)
{
    ESP_LOGI(TAG, "update system time %d:%d:%d\n", hour, minute, second);
    g_system_data.hour = hour;
    g_system_data.minute = minute;
    g_system_data.second = second;
}

void update_system_time_second()
{
    if (g_system_data.second >= 59)
    {
        g_system_data.second = 0;
        if (g_system_data.minute >= 59)
        {
            g_system_data.minute = 0;
            if (g_system_data.hour >= 23)
            {
                g_system_data.hour = 0;
            }
            else
                g_system_data.hour++;
        }
        else
            g_system_data.minute++;
    }
    else
    {
        g_system_data.second++;
    }
}
