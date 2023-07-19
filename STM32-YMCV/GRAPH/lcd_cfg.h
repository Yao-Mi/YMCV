#ifndef __LCD_CFG_H
#define __LCD_CFG_H			  	 
#include "stm32f4xx_hal.h"
#include "stdlib.h"	   
//********************************************************************************	 
//������ֻ��ѧϰʹ�ã�δ����ɣ��������������κ���;
//YMKV������
//LCD ��������	      
//��Ŀ�������Ӿ�
//��������:2019/11/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//�������׿Ƽ��������޹�˾
//********************************************************************************

#define	uint8_t unsigned char
#define	uint16_t unsigned short
#define	uint32_t unsigned int

//LCDģʽ����

#define USE_HORIZONTAL 3  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define LCD_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------LCD�˿ڶ���----------------                                    	

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

#define LCD_CMD  0	//д����
#define LCD_DATA 1	//д����

extern  uint16_t BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ
extern  uint16_t LCD_W,LCD_H;

void LCD_Init(void); 
//void LCD_Clear(uint16_t Color);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void LCD_WR_DATA8(uint8_t da); //��������-8λ����
void LCD_WR_DATA(uint16_t da);
void LCD_WR_REG(uint8_t da);

void LCD_DrawPoint(uint16_t x,uint16_t y);//����
void LCD_DrawPoint_big(uint16_t x,uint16_t y);//��һ�����
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //����


void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);//��ʾһ���ַ�
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len);//��ʾ����
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len);//��ʾ2������
void LCD_ShowString(uint16_t x,uint16_t y,const char *p);		 //��ʾһ���ַ���,16����
 
void showhanzi(uint16_t x,uint16_t y,uint8_t index);



//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


#endif  
	 



