#include "ds_ui_messpage.h"
#include "ds_data_mess.h"

static const char* TAG = "ds_ui_mess_page";

#define MESS_MAX_LEN 256
typedef struct {
    char mess[MESS_MAX_LEN];
} DS_UI_MESS_T;

static TaskHandle_t xTaskPageHandle = NULL;
static xQueueHandle g_ui_event_queue = NULL;
static xQueueHandle g_ui_mess_queue = NULL;

static char ds_showing_mess[MESS_MAX_LEN] = "Hi 你好！";

static uint8_t* page_canvas = NULL; 
static uint8_t is_showing = 0;

static void ds_draw_page_canvas(const unsigned char* bg_data); // 绘制背景
static void ds_draw_page_components(void); // 绘制组件
static void ds_ui_mess_page_update(void); // 更新页面


void ds_ui_registds_ui_mess_page(PAGE_HANDLE_T* handle)
{
    handle->page_init = ds_ui_mess_page_init;
    handle->page_destory = ds_ui_mess_page_destory;
    handle->call_event = ds_ui_mess_page_callEvent;
    handle->page_type = PAGE_TYPE_MEMU;
}

// ToDo 需要使用队列来响应事件
void ds_ui_mess_page_callEvent(UI_EVENT_T event)
{
    printf("ds_ui_mess_page_callEvent\r\n");
    ds_ui_page_manage_send_action(PAGE_TYPE_TIME); // 返回时钟页面
}

static void ds_draw_page_canvas(const unsigned char* bg_data)
{
    // 绘制背景
    EPD_HW_Init();
	EPD_SetRAMValue_BaseMap(bg_data);
	EPD_DeepSleep();
}

static void ds_draw_local_update(const unsigned char* bg_data)
{
    EPD_Dis_Part(0, 200, bg_data, EPD_2IN9BC_WIDTH, EPD_2IN9BC_HEIGHT);
    EPD_Part_Update();
    EPD_DeepSleep();
}

static void ds_draw_page_components(void)
{
    Paint_NewImage(page_canvas, EPD_2IN9BC_WIDTH, EPD_2IN9BC_HEIGHT, 0, WHITE); // 清空画布
    Paint_Clear(WHITE); // 填充白色

    Paint_DrawString_CN(18, 42, ds_showing_mess, WHITE, BLACK);

    ds_draw_page_canvas(ds_get_paint_canvas());
}

static void ds_ui_mess_page_update(void)
{
    // 接收并显示串口数据
    DS_UI_MESS_T ui_mess;
    if(xQueueReceive(g_ui_mess_queue, &ui_mess, portMAX_DELAY))
    {
        is_showing = 1;

        // printf("Receive message:%s len:%d.\n", ui_mess.mess, strlen(ui_mess.mess));

        //  char* pstr = ui_mess.mess;
        // while(*pstr != '\0')
        // {
        //     printf("0x%x ", *pstr);
        //     pstr++;
        // }
        // printf("\n");

        Paint_NewImage(page_canvas, EPD_2IN9BC_WIDTH, EPD_2IN9BC_HEIGHT, 0, WHITE); // 清空画布
        Paint_Clear(WHITE); // 填充白色
        Paint_DrawString_CN(18, 42, ui_mess.mess, WHITE, BLACK);
        // ds_draw_page_canvas(ds_get_paint_canvas());
        ds_draw_local_update(ds_get_paint_canvas());

        strcpy(ds_showing_mess, ui_mess.mess); // 保存当前显示字符串

        is_showing = 0;
    }
}

static void vPageUpdateTask(void *pvParameters)
{
    ds_draw_page_components();
    for(;;)
    {
        ds_ui_mess_page_update();
        vTaskDelay(1000 / portTICK_RATE_MS);
    }


}

void ds_ui_mess_page_init(void)
{
    printf("ds_ui_mess_page_init\r\n");
    if((page_canvas = (uint8_t*)malloc(IMAGE_SIZE)) == NULL)
    {
        printf("Failed to alloc page canvas memory.\r\n");
        return;
    }
    g_ui_event_queue = xQueueCreate(10, sizeof(UI_EVENT_T));
    g_ui_mess_queue = xQueueCreate(10, sizeof(DS_UI_MESS_T));
    xTaskCreatePinnedToCore(vPageUpdateTask, "menu_page", 4096, NULL, 5, &xTaskPageHandle, tskNO_AFFINITY);
}

void ds_ui_mess_page_destory(void)
{
    printf("ds_ui_mess_page_destory\r\n");
    if (xTaskPageHandle)
    {
        vTaskDelete(xTaskPageHandle);
        free(page_canvas); // 释放缓存内存
        page_canvas = NULL;
    }
    else
        ESP_LOGI(TAG,"Message page is not created!\n");
}

void ds_ui_show_mess(char* mess)
{
    DS_UI_MESS_T ui_mess;
    strcpy(ui_mess.mess, mess);
    printf("Receive message:%s len:%d.\n", ui_mess.mess, strlen(ui_mess.mess));
    if (ds_ui_get_now_show_page() == PAGE_TYPE_MEMU && is_showing == 0){
        xQueueSend(g_ui_mess_queue, &ui_mess, 0);
    }
}