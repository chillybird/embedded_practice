#ifndef __DS_UI_SETTINGPAGE_H__
#define __DS_UI_SETTINGPAGE_H__

#include "ds_ui_common.h"
#include "ds_ui_page_manage.h"

void ds_ui_regist_setting_page(PAGE_HANDLE_T* handle);
void ds_ui_setting_page_init();
void ds_ui_setting_page_destory();
void ds_ui_setting_page_callEvent(UI_EVENT_T event);

#endif // __DS_UI_SETTINGPAGE_H__