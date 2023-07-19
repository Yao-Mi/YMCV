#include "delay.h"
#include "stm32f4xx_hal.h"
//********************************************************************************	 
//������ֻ��ѧϰʹ�ã�δ����ɣ��������������κ���;
//YMKV������
//ʱ�亯������
//��Ŀ�������Ӿ�
//��������:2019/11/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//�������׿Ƽ��������޹�˾
//********************************************************************************

static uint8_t  us_nm=168;							//us��ʱ������			   
//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ,��Ҫ����798915us(���ֵ��2^24/us_nm@us_nm=21)
void HAL_Delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*us_nm; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
}

//��ʱnms 
//nms:0~65535
void HAL_Delay_ms(uint16_t nms)
{	 	 
	uint32_t i;
	for(i=0;i<nms;i++) HAL_Delay_us(1000);
} 
			 

