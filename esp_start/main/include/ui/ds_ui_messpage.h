#ifndef __DS_UI_MENUPAGE_H__
#define __DS_UI_MENUPAGE_H__

#include "ds_ui_common.h"
#include "ds_ui_page_manage.h"

void ds_ui_registds_ui_mess_page(PAGE_HANDLE_T* handle);
void ds_ui_mess_page_init(void);
void ds_ui_mess_page_destory(void);
void ds_ui_mess_page_callEvent(UI_EVENT_T event);
void ds_ui_show_mess(char* mess);

#endif // __DS_UI_MENUPAGE_H__