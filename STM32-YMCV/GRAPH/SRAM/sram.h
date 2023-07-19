#ifndef __SRAM_H
#define __SRAM_H															    
#include "Puse.h" 
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

void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 NumHalfwordToRead);

#endif

