#include "stm32f4xx_hal.h"
#include "sccb.h"
#include "delay.h"
//********************************************************************************	 
//本例程只供学习使用，未经许可，不得用于其它任何用途
//YMKV开发板
//OV系列摄像头 SCCB 驱动代码
//项目：妖米视觉
//创建日期:2019/11/5
//版本：V1.0
//版权所有，盗版必究。
//沈阳妖米科技责任有限公司
//********************************************************************************

//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
    SCCB_SDA=1;     //数据线高电平	   
    SCCB_SCL=1;	    //在时钟线高的时候数据线由高至低
    HAL_Delay_us(2);  
    SCCB_SDA=0;
    HAL_Delay_us(2);	 
    SCCB_SCL=0;	    //数据线恢复低电平，单操作函数必要	  
}

//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
    SCCB_SDA=0;
    HAL_Delay_us(2);	 
    SCCB_SCL=1;	
    HAL_Delay_us(2); 
    SCCB_SDA=1;	
    HAL_Delay_us(2);
}  
//产生NA信号
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
//SCCB,写入一个字节
//返回值:0,成功;1,失败. 
uint8_t SCCB_WR_Byte(uint8_t dat)
{
	uint8_t j,res;	 
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)SCCB_SDA=1;	
		else SCCB_SDA=0;
		dat<<=1;
		HAL_Delay_us(2);
		SCCB_SCL=1;	
		HAL_Delay_us(2);
		SCCB_SCL=0;		   
	}			 
	SCCB_SDA_IN();		//设置SDA为输入 
	HAL_Delay_us(2);
	SCCB_SCL=1;			//接收第九位,以判断是否发送成功
	HAL_Delay_us(2);
	if(SCCB_READ_SDA)res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	SCCB_SCL=0;		 
	SCCB_SDA_OUT();		//设置SDA为输出    
	return res;  
}	 
//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
uint8_t SCCB_RD_Byte(void)
{
	uint8_t temp=0,j;    
	SCCB_SDA_IN();		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		HAL_Delay_us(2);
		SCCB_SCL=1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		HAL_Delay_us(2);
		SCCB_SCL=0;
	}	
	SCCB_SDA_OUT();		//设置SDA为输出    
	return temp;
} 							    
//写寄存器
//返回值:0,成功;1,失败.
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data)
{
	uint8_t res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	HAL_Delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	HAL_Delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
  	SCCB_Stop();	  
  	return	res;
}		  					    
//读寄存器
//返回值:读到的寄存器值
uint8_t SCCB_RD_Reg(uint8_t reg)
{
	uint8_t val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID	  
	HAL_Delay_us(100);	 
  	SCCB_WR_Byte(reg);			//写寄存器地址	  
	HAL_Delay_us(100);	  
	SCCB_Stop();   
	HAL_Delay_us(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令	  
	HAL_Delay_us(100);
  	val=SCCB_RD_Byte();		 	//读取数据
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}















