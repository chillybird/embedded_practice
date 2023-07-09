#include <sys/param.h>
#include <stdio.h>
#include <dirent.h> // 定义了与目录操作相关的函数和数据结构
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_err.h"

#include "ds_spiffs.h"


/* This example demonstrates how to create file server
 * using esp_http_server. This file has only startup code.
 * Look in file_server.c for the implementation */

static const char *TAG="spiffs";

esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,   // This decides the maximum number of files that can be created on the storage
      .format_if_mount_failed = true
};

/* Function to initialize SPIFFS */
esp_err_t init_spiffs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    return ESP_OK;
}

void ds_spiffs_deinit(){
    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

void ds_spiffs_listFiles(void) {
    DIR *dir;
    struct dirent *ent;

    // 打开目录
    dir = opendir("/spiffs");
    if (dir == NULL) {
        ESP_LOGI(TAG, "无法打开目录\n");
        ESP_LOGI(TAG, "无法打开目录\n");
        return;
    }

    // 遍历目录中的文件名
    while ((ent = readdir(dir)) != NULL) {
        // 忽略 "." 和 ".." 目录
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        ESP_LOGI(TAG, "Fiel: %s\n", ent->d_name);
    }

    // 关闭目录
    closedir(dir);
}

int ds_spiffs_deleteEntry(const char *path)
{
    struct stat st;

    // 获取文件/文件夹属性信息
    if (stat(path, &st) == -1) {
        ESP_LOGI(TAG, "无法获取文件/文件夹信息\n");
        return -1;
    }

    // 判断文件类型
    if (S_ISREG(st.st_mode)) {
        // 普通文件，直接删除
        if (remove(path) == 0) {
            ESP_LOGI(TAG, "文件删除成功\n");
            return 0;
        } else {
            ESP_LOGI(TAG, "文件删除失败\n");
            return -1;
        }
    } else if (S_ISDIR(st.st_mode)) {
        // 文件夹，调用适当的函数进行删除（例如：rmdir、remove）
        if (rmdir(path) == 0) {
            ESP_LOGI(TAG, "文件夹删除成功\n");
            return 0;
        } else {
            ESP_LOGI(TAG, "文件夹删除失败\n");
            return -1;
        }
    } else {
        // 其他类型，不支持删除
        ESP_LOGI(TAG, "不支持的文件/文件夹类型\n");
        return -1;
    }
}

void ds_spiffs_test(){
    // Use POSIX and C standard library functions to work with files.
    // First create a file.
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen("/spiffs/hello.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello World!\n");
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Check if destination file exists before renaming
    struct stat st;
    if (stat("/spiffs/foo.txt", &st) == 0) {
        // Delete it if it exists
        unlink("/spiffs/foo.txt");
    }

    // Rename original file
    ESP_LOGI(TAG, "Renaming file");
    if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0) {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/foo.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);
}

