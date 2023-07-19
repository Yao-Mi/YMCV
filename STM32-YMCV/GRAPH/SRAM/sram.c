#include "sram.h"	  
#include "usart.h"	     
//********************************************************************************	 
//������ֻ��ѧϰʹ�ã�δ����ɣ��������������κ���;
//YMKV������
//�ⲿSRAM ��������
//��Ŀ�������Ӿ�
//��������:2019/11/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//�������׿Ƽ��������޹�˾
//********************************************************************************

//ʹ��NOR/SRAM�� Bank1.sector3,��ַλHADDR[27,26]=10 
//��IS61LV25616/IS62WV25616,��ַ�߷�ΧΪA0~A17 
//��IS61LV51216/IS62WV51216,��ַ�߷�ΧΪA0~A18
#define BANK1_SRAM3_ADDR    ((u32)(0x68000000))	

//��ָ����ַ(WriteAddr+Bank1_SRAM3_ADDR)��ʼ,����д��n���ֽ�.
//pBuffer:�ֽ�ָ��
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*(vu8*)(BANK1_SRAM3_ADDR+WriteAddr)=*pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//��ָ����ַ((WriteAddr+Bank1_SRAM3_ADDR))��ʼ,��������n���ֽ�.
//pBuffer:�ֽ�ָ��
//ReadAddr:Ҫ��������ʼ��ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*pBuffer++=*(vu8*)(BANK1_SRAM3_ADDR+ReadAddr);
		ReadAddr++;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//���Ժ���
u32 SramArray[1024*1024] __attribute__((at(0X68000000)));//����������

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


