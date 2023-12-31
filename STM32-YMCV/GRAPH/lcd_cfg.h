#ifndef __LCD_CFG_H
#define __LCD_CFG_H			  	 
#include "stm32f4xx_hal.h"
#include "stdlib.h"	   
//********************************************************************************	 
//本例程只供学习使用，未经许可，不得用于其它任何用途
//YMKV开发板
//LCD 驱动代码	      
//项目：妖米视觉
//创建日期:2019/11/5
//版本：V1.0
//版权所有，盗版必究。
//沈阳妖米科技责任有限公司
//********************************************************************************

#define	uint8_t unsigned char
#define	uint16_t unsigned short
#define	uint32_t unsigned int

//LCD模式设置

#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

//0:4线串行模式
//1:并行8080模式
#define LCD_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------LCD端口定义----------------                                    	

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET)//CLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET)

#define LCD_SDIN_Clr() HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET)//DIN
#define LCD_SDIN_Set() HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET)

#define LCD_RST_Clr() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET)//RES
#define LCD_RST_Set() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET)

#define LCD_DC_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)//DC
#define LCD_DC_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)
 		     
#define LCD_BLK_Clr()  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET)//CS
#define LCD_BLK_Set()  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET)

#define LCD_CMD  0	//写命令
#define LCD_DATA 1	//写数据

extern  uint16_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色
extern  uint16_t LCD_W,LCD_H;

void LCD_Init(void); 
//void LCD_Clear(uint16_t Color);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void LCD_WR_DATA8(uint8_t da); //发送数据-8位参数
void LCD_WR_DATA(uint16_t da);
void LCD_WR_REG(uint8_t da);

void LCD_DrawPoint(uint16_t x,uint16_t y);//画点
void LCD_DrawPoint_big(uint16_t x,uint16_t y);//画一个大点
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //读点


void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);//显示一个字符
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len);//显示数字
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len);//显示2个数字
void LCD_ShowString(uint16_t x,uint16_t y,const char *p);		 //显示一个字符串,16字体
 
void showhanzi(uint16_t x,uint16_t y,uint8_t index);



//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#endif  
	 



