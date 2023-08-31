#include "ds_ui_tomatopage.h"

static const char* TAG = "ds_ui_tomatopage";

void ds_ui_regist_tomato_page(PAGE_HANDLE_T* handle)
{
    handle->page_init = ds_ui_tomato_page_init;
    handle->page_destory = ds_ui_tomato_page_destory;
    handle->call_event = ds_ui_tomato_page_callEvent;
    handle->page_type = PAGE_TYPE_TOMATO;
}

void ds_ui_tomato_page_callEvent(UI_EVENT_T event)
{
    printf("ds_ui_tomato_page_callEvent\n");
}

void ds_ui_tomato_page_init()
{
    printf("ds_ui_tomato_page_init\n");
}

void ds_ui_tomato_page_destory()
{
    printf("ds_ui_tomato_page_destory\n");
}