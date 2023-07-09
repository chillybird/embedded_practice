
#include <string.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "ds_screen.h"
#include "ds_gpio.h"
#include "ds_spi.h"
// #include "ds_data_image.h"
// #include "ds_data_page.h"

#include "ds_data_num.h"

// Detection busy
static void lcd_chkstatus(void)
{
	int count = 0;
	unsigned char busy;
	while (1)
	{
		busy = ds_gpio_get_screen_busy();
		busy = (busy & 0x01);
		//=1 BUSY
		if (busy == 0)
			break;
		vTaskDelay(10 / portTICK_PERIOD_MS);
		count++;
		if (count >= 1000)
		{
			printf("---------------time out ---\n");
			break;
		}
	}
}

/////////////////////////////Enter deep sleep mode////////////////////////
void deep_sleep(void) // Enter deep sleep mode
{
	spi_send_cmd(0x10); // enter deep sleep
	spi_send_data(0x01);
	vTaskDelay(100 / portTICK_PERIOD_MS);
}

void refresh(void)
{
	spi_send_cmd(0x22); // Display Update Control
	spi_send_data(0xF7);
	spi_send_cmd(0x20); // Activate Display Update Sequence
	lcd_chkstatus();
}

// 图片全刷-全白函数
static void ds_screen_display_white(void)
{
	unsigned int i, k;
	spi_send_cmd(0x24); // write RAM for black(0)/white (1)
	for (k = 0; k < 250; k++)
	{
		for (i = 0; i < 25; i++)
		{
			spi_send_data(0xff);
		}
	}
}

// 图片全刷-数据函数
void ds_screen_full_display_data(const uint8_t *data)
{
	unsigned int i;
	spi_send_cmd(0x24); // write RAM for black(0)/white (1)
	for (i = 0; i < 5000; i++)
	{
		spi_send_data(*data);
		data++;
	}
}

// 接口初始化
void init_screen_interface()
{
	ds_screen_gpio_init();
	screen_spi_init();
}

void Epaper_READBUSY(void)
{
	while (1)
	{ //=1 BUSY
		if (ds_gpio_get_screen_busy() == 0)
			break;
		;
	}
}

void EPD_DeepSleep(void)
{
	spi_send_cmd(0x10); // enter deep sleep
	spi_send_data(0x01);
	vTaskDelay(100 / portTICK_PERIOD_MS);
}

void EPD_Update(void)
{
	spi_send_cmd(0x22); // Display Update Control
	spi_send_data(0xF7);
	spi_send_cmd(0x20); // Activate Display Update Sequence
	Epaper_READBUSY();
}

void EPD_Update_Fast(void)
{   
  spi_send_cmd(0x22); //Display Update Control
  spi_send_data(0xC7);   
  spi_send_cmd(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();   
}

/*When the electronic paper screen is updated, do not unplug the electronic paper to avoid damage to the screen*/
void EPD_Part_Update(void)
{
	spi_send_cmd(0x22); // Display Update Control
	spi_send_data(0xFF);
	spi_send_cmd(0x20); // Activate Display Update Sequence
	Epaper_READBUSY();
}

// SSD1681
void EPD_HW_Init(void)
{
	ds_gpio_set_screen_rst(0); // Module reset
	vTaskDelay(10 / portTICK_PERIOD_MS);
	ds_gpio_set_screen_rst(1);
	vTaskDelay(10 / portTICK_PERIOD_MS);

	Epaper_READBUSY();
	spi_send_cmd(0x12); // SWRESET
	Epaper_READBUSY();
 
	spi_send_cmd(0x01); // Driver output control
	spi_send_data(0xC7);
	spi_send_data(0x00);
	spi_send_data(0x01);

	spi_send_cmd(0x11); // data entry mode
	spi_send_data(0x01);

	spi_send_cmd(0x44); // set Ram-X address start/end position
	spi_send_data(0x00);
	spi_send_data(0x18); // 0x0C-->(18+1)*8=200

	spi_send_cmd(0x45);	 // set Ram-Y address start/end position
	spi_send_data(0xC7); // 0xC7-->(199+1)=200
	spi_send_data(0x00);
	spi_send_data(0x00);
	spi_send_data(0x00);

	spi_send_cmd(0x3C); // BorderWavefrom
	spi_send_data(0x05);

	spi_send_cmd(0x18); // Read built-in temperature sensor
	spi_send_data(0x80);

	spi_send_cmd(0x4E); // set RAM x address count to 0;
	spi_send_data(0x00);
	spi_send_cmd(0x4F); // set RAM y address count to 0X199;
	spi_send_data(0xC7);
	spi_send_data(0x00);
	Epaper_READBUSY();
}

void EPD_HW_Init_Fast(void)
{
	ds_gpio_set_screen_rst(0); // Module reset
	vTaskDelay(10 / portTICK_PERIOD_MS);
	ds_gpio_set_screen_rst(1);
	vTaskDelay(10 / portTICK_PERIOD_MS);
  
	spi_send_cmd(0x12);  //SWRESET
	Epaper_READBUSY();   

	spi_send_cmd(0x18); //Read built-in temperature sensor
	spi_send_data(0x80);  
		
	spi_send_cmd(0x22); // Load temperature value
	spi_send_data(0xB1);    
	spi_send_cmd(0x20); 
	Epaper_READBUSY();   

	spi_send_cmd(0x1A); // Write to temperature register
	spi_send_data(0x64);    
	spi_send_data(0x00);  
			
	spi_send_cmd(0x22); // Load temperature value
	spi_send_data(0x91);    
	spi_send_cmd(0x20); 
	Epaper_READBUSY();   
}

///////////////////////////Part update//////////////////////////////////////////////
// The x axis is reduced by one byte, and the y axis is reduced by one pixel.
void EPD_SetRAMValue_BaseMap(const unsigned char *datas)
{
	printf("EPD_SetRAMValue_BaseMap");

	unsigned int i;
	if (datas == NULL)
	{
		spi_send_cmd(0x24); // Write Black and White image to RAM
		for (i = 0; i < 5000; i++)
			spi_send_data(0xFF);

		spi_send_cmd(0x26); // Write Black and White image to RAM
		for (i = 0; i < 5000; i++)
			spi_send_data(0xFF);

		EPD_Update();
	}
	else
	{
		const unsigned char *datas_flag;
		datas_flag = datas;

		spi_send_cmd(0x24); // Write Black and White image to RAM
		for (i = 0; i < 5000; i++)
		{
			spi_send_data(*datas);
			datas++;
		}

		datas = datas_flag;
		spi_send_cmd(0x26); // Write Black and White image to RAM
		for (i = 0; i < 5000; i++)
		{
			spi_send_data(*datas);
			datas++;
		}
		EPD_Update();
	}
}

/**
 * 
 * 函数功能：该函数用于在电子纸屏幕上刷新指定的局部区域，传入的参数包括起始坐标、数据源以及局部区域的行列大小。
 *           首先根据起始坐标和局部区域的大小计算出需要刷新的范围，然后进行硬件复位和边框锁定操作。
 *           接下来通过SPI通信设置RAM x和y地址的起始和结束位置，以及RAM x和y地址的计数。
 *           最后将指定区域的数据写入到RAM中，实现局部区域的刷新。
 * 
 * 屏幕的坐标系原点是左下角
 * 
 * 局部更新的x轴步长为8个像素
*/
void EPD_Dis_Part(unsigned int x_start, unsigned int y_start, const unsigned char *datas, unsigned int PART_COLUMN, unsigned int PART_LINE)
{
	unsigned int i;
	unsigned int x_end, y_start1, y_start2, y_end1, y_end2;
	x_start = x_start / 8; // 将x_start调整为字节对齐
	x_end = x_start + PART_LINE / 8 - 1;

	// y_start1和y_start2是用来表示RAM y地址起始位置, 分别指的是高8位和低8位
	y_start1 = 0;
	y_start2 = y_start;
	if (y_start >= 256)
	{
		y_start1 = y_start2 / 256;
		y_start2 = y_start2 % 256;
	}
	
	y_end1 = 0;
	y_end2 = y_start + PART_COLUMN - 1;
	if (y_end2 >= 256)
	{
		y_end1 = y_end2 / 256;
		y_end2 = y_end2 % 256;
	}

	// Add hardware reset to prevent background color change
	ds_gpio_set_screen_rst(0); // Module reset
	vTaskDelay(10 / portTICK_PERIOD_MS);
	ds_gpio_set_screen_rst(1);
	vTaskDelay(10 / portTICK_PERIOD_MS);
	// Lock the border to prevent flashing
	spi_send_cmd(0x3C); // BorderWavefrom,
	spi_send_data(0x80);

	spi_send_cmd(0x44);		 // set RAM x address start/end, in page 35
	spi_send_data(x_start);	 // RAM x address start at 00h;
	spi_send_data(x_end);	 // RAM x address end at 0fh(15+1)*8->128
	spi_send_cmd(0x45);		 // set RAM y address start/end, in page 35
	spi_send_data(y_start2); // RAM y address start at 0127h;
	spi_send_data(y_start1); // RAM y address start at 0127h;
	spi_send_data(y_end2);	 // RAM y address end at 00h;
	spi_send_data(y_end1);	 // ????=0

	spi_send_cmd(0x4E); // set RAM x address count to 0;
	spi_send_data(x_start);
	spi_send_cmd(0x4F); // set RAM y address count to 0X127;
	spi_send_data(y_start2);
	spi_send_data(y_start1);

	spi_send_cmd(0x24); // Write Black and White image to RAM
	for (i = 0; i < PART_COLUMN * PART_LINE / 8; i++)
	{
		// spi_send_data(~*datas); // ~
		spi_send_data(*datas); // ~
		datas++;
	}
}

void EPD_WhiteScreen_White(void)
{
	unsigned int i, k;
	spi_send_cmd(0x24); // write RAM for black(0)/white (1)
	for (k = 0; k < 250; k++)
	{
		for (i = 0; i < 25; i++)
		{
			spi_send_data(0xff);
		}
	}
	EPD_Update();
}

#define HEI

#ifdef HEI


void test_hei(void)
{
	ds_screen_full_display_data(num_bg);
	EPD_Update();
	// EPD_Update_Fast();
}

#endif

void test_SSD1681()
{
	EPD_HW_Init();			 // Electronic paper initialization
	// EPD_HW_Init_Fast();
	// EPD_SetRAMValue_BaseMap(NULL);
	EPD_SetRAMValue_BaseMap(num_bg);
	EPD_DeepSleep();

	// test_hei();

	uint8_t i = 0;

	uint8_t offset_y = 1;

	for (i = 0; i < 10; i++)
	{
		EPD_Dis_Part(32, 168 - offset_y,hour0_bgImg[i],32,32); //x,y,DATA,Resolution 32*32
		EPD_Dis_Part(64, 168 - offset_y,hour1_bgImg[i],32,32); //x,y,DATA,Resolution 32*32
		EPD_Dis_Part(96, 168 - offset_y, colon_bg_10Img,32,16); //x,y,DATA,Resolution 32*32
		EPD_Dis_Part(112, 168 - offset_y,min0_bgImg[i],32,32); //x,y,DATA,Resolution 32*32
		EPD_Dis_Part(144, 168 - offset_y,min1_bgImg[i],32,32); //x,y,DATA,Resolution 32*32

		EPD_Part_Update();
		EPD_DeepSleep();
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
