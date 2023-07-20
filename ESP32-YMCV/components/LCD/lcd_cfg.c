#include "lcd_cfg.h"
#include "stdlib.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/spi_master.h"

uint16_t BACK_COLOR = WHITE, POINT_COLOR = BLACK; // 背景色，画笔色
extern void LCD_Clear(uint16_t Color);

spi_device_handle_t lcd_spi = NULL;

/******************************************************************************
	  函数说明：LCD串行数据写入函数
	  入口数据：dat  要写入的串行数据
	  返回值：  无
******************************************************************************/
static inline void LCD_Writ_Bus(uint8_t dat)
{
	spi_transaction_t t = {0};

	t.length = 1 * 8;	// 数据长度 为 8 bits
	t.tx_buffer = &dat; // 数据的buff
	// 发送数据
	esp_err_t ret = spi_device_polling_transmit(lcd_spi, &t); // Transmit!
	assert(ret == ESP_OK);
}

void LCD_WR_Data_Buff(uint8_t *datap, uint32_t len)
{
	LCD_DC_Set(); // 写数据

	spi_transaction_t t = {0};

	t.length = len * 8;	 // 数据长度 为 8 bits
	t.tx_buffer = datap; // 数据的buff
	// 发送数据
	esp_err_t ret = spi_device_polling_transmit(lcd_spi, &t); // Transmit!
	assert(ret == ESP_OK);
}

/******************************************************************************
	  函数说明：LCD写入命令
	  入口数据：dat 写入的命令
	  返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr(); // 写命令
	LCD_Writ_Bus(dat);
}

/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_Set(); // 写数据
	LCD_Writ_Bus(dat);
}

/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	//LCD_DC_Set(); // 写数据
	// LCD_Writ_Bus(dat >> 8);
	// LCD_Writ_Bus(dat);
	uint8_t dbuf[2] = {0};
	dbuf[0] = dat >> 8;
	dbuf[1] = dat;
	LCD_WR_Data_Buff(dbuf,2);
}

void LCD_WR_DATA2(uint16_t dat1,uint16_t dat2)
{
	uint8_t dbuf[4] = {0};
	dbuf[0] = dat1 >> 8;
	dbuf[1] = dat1;
	dbuf[2] = dat2 >> 8;
	dbuf[3] = dat2;
	LCD_WR_Data_Buff(dbuf,4);
}
/******************************************************************************
	  函数说明：设置起始和结束地址
	  入口数据：x1,x2 设置列的起始和结束地址
				y1,y2 设置行的起始和结束地址
	  返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if (USE_HORIZONTAL == 0)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		// LCD_WR_DATA(x1);
		// LCD_WR_DATA(x2);
		LCD_WR_DATA2(x1,x2);
		LCD_WR_REG(0x2b); // 行地址设置
		// LCD_WR_DATA(y1);
		// LCD_WR_DATA(y2);
		LCD_WR_DATA2(y1,y2);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else if (USE_HORIZONTAL == 1)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		// LCD_WR_DATA(x1);
		// LCD_WR_DATA(x2);
		LCD_WR_DATA2(x1,x2);
		LCD_WR_REG(0x2b); // 行地址设置
		// LCD_WR_DATA(y1 + 80);
		// LCD_WR_DATA(y2 + 80);
		LCD_WR_DATA2(y1 + 80,y2 + 80);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else if (USE_HORIZONTAL == 2)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		// LCD_WR_DATA(x1);
		// LCD_WR_DATA(x2);
		LCD_WR_DATA2(x1,x2);
		LCD_WR_REG(0x2b); // 行地址设置
		// LCD_WR_DATA(y1);
		// LCD_WR_DATA(y2);
		LCD_WR_DATA2(y1,y2);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else
	{
		LCD_WR_REG(0x2a); // 列地址设置
		// LCD_WR_DATA(x1 + 80);
		// LCD_WR_DATA(x2 + 80);
		LCD_WR_DATA2(x1 + 80,x2 + 80);
		LCD_WR_REG(0x2b); // 行地址设置
		// LCD_WR_DATA(y1);
		// LCD_WR_DATA(y2);
		LCD_WR_DATA2(y1,y2);
		LCD_WR_REG(0x2c); // 储存器写
	}
}

void lcd_gpio_reset()
{
	// IO2
	gpio_pad_select_gpio(LCD_PIN_NUM_BCKL);					// 选择一个GPIO，
	gpio_set_direction(LCD_PIN_NUM_BCKL, GPIO_MODE_OUTPUT); // 把这个GPIO作为输出
	// IO12
	gpio_pad_select_gpio(LCD_PIN_NUM_CLK);
	gpio_set_direction(LCD_PIN_NUM_CLK, GPIO_MODE_OUTPUT);
	// IO13
	gpio_pad_select_gpio(LCD_PIN_NUM_MOSI);
	gpio_set_direction(LCD_PIN_NUM_MOSI, GPIO_MODE_OUTPUT);
	// IO14
	gpio_pad_select_gpio(LCD_PIN_NUM_DC);
	gpio_set_direction(LCD_PIN_NUM_DC, GPIO_MODE_OUTPUT);
	// IO15
	gpio_pad_select_gpio(LCD_PIN_NUM_RST);
	gpio_set_direction(LCD_PIN_NUM_RST, GPIO_MODE_OUTPUT);

	// // 配置引脚为 IOMUX 引脚
	// gpio_matrix_out(LCD_PIN_NUM_CLK, SPI_CLK_OUT_IDX, false, false);
	// gpio_matrix_out(LCD_PIN_NUM_MOSI, SPI_MISO_OUT_IDX, false, false);
	// gpio_matrix_out(LCD_PIN_NUM_DC, SPI_CS0_OUT_IDX, false, false);
	// gpio_matrix_out(LCD_PIN_NUM_RST, SPI_CS1_OUT_IDX, false, false);
	// gpio_matrix_out(LCD_PIN_NUM_BCKL, SPI_WP_OUT_IDX, false, false);



	// 初始化 CLK 引脚电平拉高 ,否则无法初始化成功,有巍峨用的三 SPI mode 3,空闲时为高电平
	gpio_set_level(LCD_PIN_NUM_CLK, 1);
}

void lcd_spi_init()
{

	// spi初始化
	esp_err_t ret;

#define LCD_PIN_NUM_MISO -1 // 不使用 MISO 引脚
	spi_bus_config_t buscfg = {
		.miso_io_num = LCD_PIN_NUM_MISO,
		.mosi_io_num = LCD_PIN_NUM_MOSI,
		.sclk_io_num = LCD_PIN_NUM_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 0,
		.flags = 0,
		.intr_flags = 0};

#define LCD_PIN_NUM_CS -1 // 不使用 CS 引脚
	spi_device_interface_config_t devcfg = {
		.flags = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_NO_DUMMY, // 使用半双工模式，并忽略虚拟位检查，从而提高速度
		.clock_speed_hz = 26 * 1000 * 1000,					  // 设置 SPI 时钟速度 60MHZ，更高就无法显示了
		// SPI 模式 3
		// CPOL = 1：空闲时是高电平
		// CPHA = 1：数据在第2个跳变沿（上升沿）采样
		.mode = 3,
		.spics_io_num = LCD_PIN_NUM_CS, // 不使用 CS 引脚
		.queue_size = 1,
		.pre_cb = NULL};

	// 初始化 SPI 总线
	ret = spi_bus_initialize(LCD_SPI_HOST, &buscfg, LCD_DMA_CHAN);
	ESP_ERROR_CHECK(ret);

	// 添加 SPI 设备
	ret = spi_bus_add_device(LCD_SPI_HOST, &devcfg, &lcd_spi);
	ESP_ERROR_CHECK(ret);
}

void LCD_GPIO_Config()
{
	// gpio 重置
	lcd_gpio_reset();

	// spi 初始化
	lcd_spi_init();
}

void LCD_Init(void)
{
	// 将GPIO配置为输出
	LCD_GPIO_Config();

	LCD_RST_Clr(); // 复位
	LCD_Delay_ms(20);
	LCD_RST_Set();
	LCD_Delay_ms(20);
	LCD_BLK_Set(); // 打开背光

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11);  // Sleep out
	LCD_Delay_ms(120); // Delay 120ms

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x36);
	if (USE_HORIZONTAL == 0)
		LCD_WR_DATA8(0x00);
	else if (USE_HORIZONTAL == 1)
		LCD_WR_DATA8(0xC0);
	else if (USE_HORIZONTAL == 2)
		LCD_WR_DATA8(0x70);
	else
		LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x19);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x12);

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);

	LCD_WR_REG(0xC6);
	LCD_WR_DATA8(0x0F);

	LCD_WR_REG(0xD0);
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);
	LCD_WR_REG(0x21);

	LCD_WR_REG(0x29);

	// 清除屏幕
	LCD_Clear(WHITE);
}

////在指定位置显示一个汉字(32*33大小)
////dcolor为内容颜色，gbcolor为背静颜色
// void showhanzi(uint16_t x,uint16_t y,unsigned char index)
//{
//	unsigned char i,j;
//	unsigned char *temp=hanzi;
//   Address_set(x,y,x+31,y+31); //设置区域
//	temp+=index*128;
//	for(j=0;j<128;j++)
//	{
//		for(i=0;i<8;i++)
//		{
//		 	if((*temp&(1<<i))!=0)
//			{
//				LCD_WR_DATA(POINT_COLOR);
//			}
//			else
//			{
//				LCD_WR_DATA(BACK_COLOR);
//			}
//		}
//		temp++;
//	 }
// }

// 画点
// POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x, uint16_t y)
{
	LCD_Address_Set(x, y, x, y); // 设置光标位置
	LCD_WR_DATA(POINT_COLOR);
}
// 画一个大点
// POINT_COLOR:此点的颜色
void LCD_DrawPoint_big(uint16_t x, uint16_t y)
{
	LCD_Address_Set(x - 1, y - 1, x + 1, y + 1); // 设置光标位置
	// 填充buff
	uint8_t clbuff[3 * 3 * 2];
	uint8_t *pbf = clbuff;
	for (int i = 0; i < 3 * 3; i++)
	{
		*pbf++ = POINT_COLOR >> 8;
		*pbf++ = POINT_COLOR;
		// LCD_WR_DATA(POINT_COLOR);
	}
	LCD_WR_Data_Buff(clbuff, sizeof(clbuff));
}

//
////m^n函数
// uint32_t mypow(uint8_t m,uint8_t n)
//{
//	uint32_t result=1;
//	while(n--)result*=m;
//	return result;
// }
////显示2个数字
////x,y :起点坐标
////len :数字的位数
////color:颜色
////num:数值(0~4294967295);
// void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len)
//{
//	uint8_t t,temp;
//	uint8_t enshow=0;
//	num=(uint16_t)num;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/mypow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				LCD_ShowChar(x+8*t,y,' ',0);
//				continue;
//			}else enshow=1;
//
//		}
//	 	LCD_ShowChar(x+8*t,y,temp+48,0);
//	}
// }
////显示2个数字
////x,y:起点坐标
////num:数值(0~99);
// void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len)
//{
//	uint8_t t,temp;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/mypow(10,len-t-1))%10;
//	 	LCD_ShowChar(x+8*t,y,temp+'0',0);
//	}
// }

// #include "ympic.h"
// extern void YMKV_show_pic(uint16_t x,uint16_t y,uint16_t Width,uint16_t Height,uint16_t *pic);
////测试函数
// void LCD_test()
//{
//	LCD_Clear(WHITE);
//	LCD_ShowString(20,20,"LCD OK!");
//	YMKV_show_pic(10,60,200,126,(uint16_t*)gImage_ympic);
// }
