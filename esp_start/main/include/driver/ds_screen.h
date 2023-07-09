#ifndef _DS_SCREEN_H_
#define _DS_SCREEN_H_


void init_screen_interface(void);
void ds_screen_init();
void ds_screen_clean_white();

void ds_screen_full_display_data(const uint8_t *data);

void test_SSD1681(void);

void deep_sleep();

// added
void EPD_HW_Init(void);
void EPD_SetRAMValue_BaseMap(const unsigned char *datas);
void EPD_DeepSleep(void);
void EPD_Dis_Part(unsigned int x_start, unsigned int y_start, const unsigned char *datas, unsigned int PART_COLUMN, unsigned int PART_LINE);
void EPD_Part_Update(void);

#endif

