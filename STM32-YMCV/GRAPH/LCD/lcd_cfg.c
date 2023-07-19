#include "stm32f4xx_hal.h"
#include "delay.h"
#include "lcd_cfg.h"
#include "stdlib.h"

uint16_t BACK_COLOR=WHITE, POINT_COLOR=BLACK;   //背景色，画笔色

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
	u8 i;
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_SDIN_Set();
		}
		else
		{
		   LCD_SDIN_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//写数据
}

/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+80);
		LCD_WR_DATA(x2+80);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
}


void LCD_Init(void)
{
	LCD_RST_Clr();//复位
	HAL_Delay_ms(20);
	LCD_RST_Set();
	HAL_Delay_ms(20);
	LCD_BLK_Set();//打开背光
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	HAL_Delay_ms(120);              //Delay 120ms 
	
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

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
	
	//LCD_Clear(WHITE);
	LCD_Address_Set(0,0,LCD_W - 1,LCD_H - 1);//设置光标位置 
	for(int i=0 , imax = LCD_W * LCD_H;i<imax;i++)
		LCD_WR_DATA(WHITE);
} 

////在指定位置显示一个汉字(32*33大小)
////dcolor为内容颜色，gbcolor为背静颜色
//void showhanzi(uint16_t x,uint16_t y,unsigned char index)	
//{  
//	unsigned char i,j;
//	unsigned char *temp=hanzi;    
//  Address_set(x,y,x+31,y+31); //设置区域      
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
//}

//画点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR);
} 	 
//画一个大点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint_big(uint16_t x,uint16_t y)
{
	LCD_Address_Set(x-1,y-1,x+1,y+1);//设置光标位置 
	for(int i=0;i<3*3;i++)
		LCD_WR_DATA(POINT_COLOR);
} 


// 
////m^n函数
//uint32_t mypow(uint8_t m,uint8_t n)
//{
//	uint32_t result=1;	 
//	while(n--)result*=m;    
//	return result;
//}			 
////显示2个数字
////x,y :起点坐标	 
////len :数字的位数
////color:颜色
////num:数值(0~4294967295);	
//void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len)
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
//} 
////显示2个数字
////x,y:起点坐标
////num:数值(0~99);	 
//void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len)
//{         	
//	uint8_t t,temp;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/mypow(10,len-t-1))%10;
//	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
//	}
//} 

//#include "ympic.h"
//extern void YMKV_show_pic(uint16_t x,uint16_t y,uint16_t Width,uint16_t Height,uint16_t *pic);
////测试函数
//void LCD_test()
//{
//	LCD_Clear(WHITE);
//	LCD_ShowString(20,20,"LCD OK!");
//	YMKV_show_pic(10,60,200,126,(uint16_t*)gImage_ympic);
//}

