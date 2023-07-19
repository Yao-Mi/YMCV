#include "stdio.h"
#include "usart.h"
#include "Puse.h"	

//********************************************************************************	 
//本例程只供学习使用，未经许可，不得用于其它任何用途
//YMKV开发板
//串口0 初始化	   
//项目：妖米视觉
//创建日期:2019/11/5
//版本：V1.0
//版权所有，盗版必究。
//沈阳妖米科技责任有限公司
//********************************************************************************

//重定义fputc函数 ,printf重定向
int fputc(int ch, FILE *f)
{ 
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xffff);	
	return ch;
}
 
//重定义fgetc函数, scanf\getchar重定向  
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart3, &ch, 1, 0xffff);
  return ch;
}

void uart_test()
{
	uint8_t data=getchar();
	printf("%c",data);
}