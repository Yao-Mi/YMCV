#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
/*******************************************************************************************
    *
    * ˵��  ������չʾ�� �ڲ��ƻ�ԭʼ������£������ݷ�װΪͼƬ���Լ�ͼƬ�ı��ݣ���ͼƬ��ʾ
    * 
    ***************************************************************************************/
int main()
{
    LCD_Init(640, 480);
 
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�
    uint8* myData = CV_malloc1(sizeof(linglong00));//�������CV_malloc1ΪͼƬ���������ڴ�
    //�����ݿ�����������
    CV_memcpy(myData, linglong00, sizeof(linglong00));
   //�����ݷ�װΪͼƬ��ʽ 
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8 *)myData, 500, 255, CVRgb16Type);


    //���¿���һ�ݸ�ͼ
    CVIMAGE myImage1 = YMCV_Creat_Img_BackUp(myImage0);
    //��ʾͼƬ
    LCD_show_image(0,0, myImage1);
    //��ͣ
    PAUSE();

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);


    PAUSE();
    LCD_Destory();
    return 0;
}





