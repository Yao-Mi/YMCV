#ifndef __SCCB_H
#define __SCCB_H
#include "stm32f4xx_hal.h"
#include "Puse.h"
//********************************************************************************	 
//������ֻ��ѧϰʹ�ã�δ����ɣ��������������κ���;
//YMKV������
//SCCB  ��������	   
//��Ŀ�������Ӿ�
//��������:2019/11/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//�������׿Ƽ��������޹�˾
//********************************************************************************


//IO��������
#define SCCB_SDA_IN()  {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=0<<7*2;}	//PD7 ����
#define SCCB_SDA_OUT() {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=1<<7*2;} 	//PD7 ���


//IO��������	 
#define SCCB_SCL    		PDout(6)	 	//SCL
#define SCCB_SDA    		PDout(7) 		//SDA	 

#define SCCB_READ_SDA    	PDin(7)  		//����SDA    
#define SCCB_ID   			0X60  			//OV2640��ID

///////////////////////////////////////////
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void); 
#endif













