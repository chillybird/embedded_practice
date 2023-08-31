#ifndef __DS_UI_PAGE_MANAGE_H__
#define __DS_UI_PAGE_MANAGE_H__

#include "ds_ui_common.h"
#include "freertos/queue.h"

#include "ds_ui_messpage.h"
#include "ds_ui_settingpage.h"
#include "ds_ui_timepage.h"
#include "ds_ui_tomatopage.h"


PAGE_TYPE_E ds_ui_get_now_show_page(void);
void ds_ui_page_manage_send_event(TP_ACTION_E key, uint8_t touch_x, uint8_t touch_y);
void ds_ui_page_manage_send_action(PAGE_TYPE_E action);
void ds_ui_page_manage_init(void);
void ds_ui_page_register(void);

#endif // DS_UI_PAGE_MANAGE_H__