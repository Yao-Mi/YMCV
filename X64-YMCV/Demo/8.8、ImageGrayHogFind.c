#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_BasicObjCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� �Ҷ�ͼΪ����չʾ��HOG��������ȡЧ��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------HOG�������--------------------------------------------------*/
    CVVECTORS_F32 hogDescribe= YMCV_Gray_Find_Hog_Describe(myImage0);

    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//��ʾͼƬ0
    LCDHogHistShow(hogDescribe, 300, 0,1);//��ʾHOG������

    //��Ϣ��ʾ
    sprintf(myInform, " HOGֱ��ͼ ");
    LCD_ShowString(100, 310, myInform);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_Vector_F32_Free(hogDescribe);//�����ڴ��ͷ�

    PAUSE();
    LCD_Destory();
    return 0;
}





