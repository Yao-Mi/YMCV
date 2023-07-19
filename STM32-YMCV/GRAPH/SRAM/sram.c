#include "sram.h"	  
#include "usart.h"	     
//********************************************************************************	 
//本例程只供学习使用，未经许可，不得用于其它任何用途
//YMKV开发板
//外部SRAM 驱动代码
//项目：妖米视觉
//创建日期:2019/11/5
//版本：V1.0
//版权所有，盗版必究。
//沈阳妖米科技责任有限公司
//********************************************************************************

//使用NOR/SRAM的 Bank1.sector3,地址位HADDR[27,26]=10 
//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
#define BANK1_SRAM3_ADDR    ((u32)(0x68000000))	

//在指定地址(WriteAddr+Bank1_SRAM3_ADDR)开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*(vu8*)(BANK1_SRAM3_ADDR+WriteAddr)=*pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//在指定地址((WriteAddr+Bank1_SRAM3_ADDR))开始,连续读出n个字节.
//pBuffer:字节指针
//ReadAddr:要读出的起始地址
//n:要写入的字节数
void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*pBuffer++=*(vu8*)(BANK1_SRAM3_ADDR+ReadAddr);
		ReadAddr++;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//测试函数
u32 SramArray[1024*1024] __attribute__((at(0X68000000)));//测试用数组

#include "stdio.h"
void my_sram_mytst()
{
	u32 i=0;
	u8 temp=0;
	u8 sval=0;
	
	printf("\r\nEX mem tst :w\r\n");
	for(i=0;i<1024*1024;i+=4096)
	{
		printf(" %d \r",temp);
		FSMC_SRAM_WriteBuffer(&temp,i,1);
		temp=0;
		FSMC_SRAM_ReadBuffer(&temp,i,1);
		printf(" %d \r\n",temp);
		temp++;
	}
	printf("\r\nEX mem tst :r\r\n");
	for(i=0;i<1024*1024;i+=4096)
	{
		temp=0;
		FSMC_SRAM_ReadBuffer(&temp,i,1);
		printf(" %d \r\n",temp);
	}
}


