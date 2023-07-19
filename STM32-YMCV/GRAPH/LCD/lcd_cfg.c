#include "stm32f4xx_hal.h"
#include "delay.h"
#include "lcd_cfg.h"
#include "stdlib.h"

uint16_t BACK_COLOR=WHITE, POINT_COLOR=BLACK;   //����ɫ������ɫ

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
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
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}

/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+80);
		LCD_WR_DATA(x2+80);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
}


void LCD_Init(void)
{
	LCD_RST_Clr();//��λ
	HAL_Delay_ms(20);
	LCD_RST_Set();
	HAL_Delay_ms(20);
	LCD_BLK_Set();//�򿪱���
	
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
	LCD_Address_Set(0,0,LCD_W - 1,LCD_H - 1);//���ù��λ�� 
	for(int i=0 , imax = LCD_W * LCD_H;i<imax;i++)
		LCD_WR_DATA(WHITE);
} 

////��ָ��λ����ʾһ������(32*33��С)
////dcolorΪ������ɫ��gbcolorΪ������ɫ
//void showhanzi(uint16_t x,uint16_t y,unsigned char index)	
//{  
//	unsigned char i,j;
//	unsigned char *temp=hanzi;    
//  Address_set(x,y,x+31,y+31); //��������      
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

//����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(POINT_COLOR);
} 	 
//��һ�����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint_big(uint16_t x,uint16_t y)
{
	LCD_Address_Set(x-1,y-1,x+1,y+1);//���ù��λ�� 
	for(int i=0;i<3*3;i++)
		LCD_WR_DATA(POINT_COLOR);
} 


// 
////m^n����
//uint32_t mypow(uint8_t m,uint8_t n)
//{
//	uint32_t result=1;	 
//	while(n--)result*=m;    
//	return result;
//}			 
////��ʾ2������
////x,y :�������	 
////len :���ֵ�λ��
////color:��ɫ
////num:��ֵ(0~4294967295);	
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
////��ʾ2������
////x,y:�������
////num:��ֵ(0~99);	 
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
////���Ժ���
//void LCD_test()
//{
//	LCD_Clear(WHITE);
//	LCD_ShowString(20,20,"LCD OK!");
//	YMKV_show_pic(10,60,200,126,(uint16_t*)gImage_ympic);
//}

