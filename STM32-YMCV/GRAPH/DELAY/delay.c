#include "delay.h"
#include "stm32f4xx_hal.h"
//********************************************************************************	 
//本例程只供学习使用，未经许可，不得用于其它任何用途
//YMKV开发板
//时间函数定义
//项目：妖米视觉
//创建日期:2019/11/5
//版本：V1.0
//版权所有，盗版必究。
//沈阳妖米科技责任有限公司
//********************************************************************************

static uint8_t  us_nm=168;							//us延时倍乘数			   
//延时nus
//nus为要延时的us数.	
//注意:nus的值,不要大于798915us(最大值即2^24/us_nm@us_nm=21)
void HAL_Delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*us_nm; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}

//延时nms 
//nms:0~65535
void HAL_Delay_ms(uint16_t nms)
{	 	 
	uint32_t i;
	for(i=0;i<nms;i++) HAL_Delay_us(1000);
} 
			 

