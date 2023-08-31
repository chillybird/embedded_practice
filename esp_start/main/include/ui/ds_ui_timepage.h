#ifndef __DS_UI_TIMEPAGE_H__
#define __DS_UI_TIMEPAGE_H__

#include "ds_ui_common.h"
#include "ds_ui_page_manage.h"

void ds_ui_regist_time_page(PAGE_HANDLE_T* handle);
void ds_ui_timepage_updatetime(void);
void ds_ui_timepage_init(void); // 初始化页面
void ds_ui_timepage_destory(void); // 销毁页面
void ds_ui_time_page_callEvent(UI_EVENT_T event); // 响应点击事件

#endif // DS_UI_TIMEPAGE_H__