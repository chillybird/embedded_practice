/**
 * 注册新页面的方法：
 * 1. 在 PAGE_TYPE_E 中添加新的页面，并在 manage 文件中添加响应的页面头文件
 * 2. 在页面文件中定义 page_regist()、page_init() 以及 page_destory() 函数
 * 3. 在 ds_ui_page_register() 调用页面的 page_regist() 函数注册页面
 * 
 * 通过修改.c文件中的 page_default 来设置默认显示的页面
*/

#ifndef __DS_UI_COMMON_H__
#define __DS_UI_COMMON_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/queue.h"

#include "ds_system_data.h"
#include "ds_screen.h"
#include "ds_paint.h"

#define EPD_2IN9BC_WIDTH 200
#define EPD_2IN9BC_HEIGHT 200
#define IMAGE_SIZE (((EPD_2IN9BC_WIDTH % 8 == 0) ? (EPD_2IN9BC_WIDTH / 8) : (EPD_2IN9BC_WIDTH / 8 + 1)) * EPD_2IN9BC_HEIGHT)

typedef enum
{
	PAGE_TYPE_MEMU = 0,
	PAGE_TYPE_TIME,
	PAGE_TYPE_SETTING,
	PAGE_TYPE_TOMATO,
	PAGE_COUNT, // 页面计数
} PAGE_TYPE_E;

typedef struct {
    TP_ACTION_E key;
    uint8_t touch_x;
    uint8_t touch_y;
    PAGE_TYPE_E action;
} UI_EVENT_T;

typedef void (*PageInitFunc)(void); // 页面初始化函数
typedef void (*PageDestoryFunc)(void); // 页面销毁函数
typedef void (*CallEventFunc)(UI_EVENT_T); // 页面事件响应函数，队列实现

typedef struct
{
	PageInitFunc page_init;
	PageDestoryFunc page_destory;
    CallEventFunc call_event;
	PAGE_TYPE_E page_type;
} PAGE_HANDLE_T; // 定义 page 句柄

typedef struct
{
	PAGE_TYPE_E now_show_page;
} PAGE_MANAGE_T;

typedef struct {
    char* pageBG;
} PAGE_DATA;

#endif // __DS_UI_COMMON_H__
