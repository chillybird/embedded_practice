#ifndef _DS_SPIFFS_H_
#define _DS_SPIFFS_H_

esp_err_t init_spiffs(void);
void ds_spiffs_deinit(void);
void ds_spiffs_test(void);
void ds_spiffs_listFiles(void); // 列出spiffs中保存的文件名
int ds_spiffs_deleteEntry(const char *path); // 删除文件

#endif

