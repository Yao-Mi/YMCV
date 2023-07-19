#include "stm32f4xx_hal.h"
#include "sccb.h"
#include "delay.h"
//********************************************************************************	 
//������ֻ��ѧϰʹ�ã�δ����ɣ��������������κ���;
//YMKV������
//OVϵ������ͷ SCCB ��������
//��Ŀ�������Ӿ�
//��������:2019/11/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//�������׿Ƽ��������޹�˾
//********************************************************************************

//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
    SCCB_SDA=1;     //�����߸ߵ�ƽ	   
    SCCB_SCL=1;	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    HAL_Delay_us(2);  
    SCCB_SDA=0;
    HAL_Delay_us(2);	 
    SCCB_SCL=0;	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
    SCCB_SDA=0;
    HAL_Delay_us(2);	 
    SCCB_SCL=1;	
    HAL_Delay_us(2); 
    SCCB_SDA=1;	
    HAL_Delay_us(2);
}  
//����NA�ź�
void SCCB_No_Ack(void)
{
	HAL_Delay_us(2);
	SCCB_SDA=1;	
	SCCB_SCL=1;	
	HAL_Delay_us(2);
	SCCB_SCL=0;	
	HAL_Delay_us(2);
	SCCB_SDA=0;	
	HAL_Delay_us(2);
}
//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
uint8_t SCCB_WR_Byte(uint8_t dat)
{
	uint8_t j,res;	 
	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80)SCCB_SDA=1;	
		else SCCB_SDA=0;
		dat<<=1;
		HAL_Delay_us(2);
		SCCB_SCL=1;	
		HAL_Delay_us(2);
		SCCB_SCL=0;		   
	}			 
	SCCB_SDA_IN();		//����SDAΪ���� 
	HAL_Delay_us(2);
	SCCB_SCL=1;			//���յھ�λ,���ж��Ƿ��ͳɹ�
	HAL_Delay_us(2);
	if(SCCB_READ_SDA)res=1;  //SDA=1����ʧ�ܣ�����1
	else res=0;         //SDA=0���ͳɹ�������0
	SCCB_SCL=0;		 
	SCCB_SDA_OUT();		//����SDAΪ���    
	return res;  
}	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
uint8_t SCCB_RD_Byte(void)
{
	uint8_t temp=0,j;    
	SCCB_SDA_IN();		//����SDAΪ����  
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		HAL_Delay_us(2);
		SCCB_SCL=1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		HAL_Delay_us(2);
		SCCB_SCL=0;
	}	
	SCCB_SDA_OUT();		//����SDAΪ���    
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data)
{
	uint8_t res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
	HAL_Delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	HAL_Delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//д����	 
  	SCCB_Stop();	  
  	return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
uint8_t SCCB_RD_Reg(uint8_t reg)
{
	uint8_t val=0;
	SCCB_Start(); 				//����SCCB����
	SCCB_WR_Byte(SCCB_ID);		//д����ID	  
	HAL_Delay_us(100);	 
  	SCCB_WR_Byte(reg);			//д�Ĵ�����ַ	  
	HAL_Delay_us(100);	  
	SCCB_Stop();   
	HAL_Delay_us(100);	   
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//���Ͷ�����	  
	HAL_Delay_us(100);
  	val=SCCB_RD_Byte();		 	//��ȡ����
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}















