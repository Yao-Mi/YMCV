#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_FormCge.h"
/*******************************************************************************************
	*
	* ˵��  ������չʾ�� ͼƬ�������Լ�ͼƬ�ĸ�ʽת��
	*
	***************************************************************************************/
int main()
{
	LCD_Init(1000, 480);

	//Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
	uint8* myData = CV_malloc1(sizeof(linglong00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
	CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
	CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//�Ҷ�ͼ
	
	//��ʾͼƬ0
	LCD_show_image(0, 0, myImage0);
	//��ͣ
	PAUSE();

	//��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
	CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);

	//ת��ֵ��ͼ
	YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_GrayToDotBinary);
	//��ʾͼƬ1
	LCD_show_image(500, 0, myImage1);
	//��ͣ
	PAUSE();


	//ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
	YMCV_Free_ImgFree(myImage0);
	YMCV_Free_ImgFree(myImage1);


	PAUSE();
	LCD_Destory();
	return 0;
}



