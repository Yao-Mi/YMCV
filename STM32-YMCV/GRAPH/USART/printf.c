#include "stdio.h"
#include "usart.h"
#include "Puse.h"	

//********************************************************************************	 
//������ֻ��ѧϰʹ�ã�δ����ɣ��������������κ���;
//YMKV������
//����0 ��ʼ��	   
//��Ŀ�������Ӿ�
//��������:2019/11/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//�������׿Ƽ��������޹�˾
//********************************************************************************

//�ض���fputc���� ,printf�ض���
int fputc(int ch, FILE *f)
{ 
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xffff);	
	return ch;
}
 
//�ض���fgetc����, scanf\getchar�ض���  
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