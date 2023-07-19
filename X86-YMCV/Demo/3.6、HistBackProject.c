#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼ�涨��Ϊ����չʾ�˽�һ��ͼ���ֱ��ͼ������һ��ͼʹ�õķ�����ֱ��ͼ��ʾ
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//�Ҷ�ͼ1

    CVrect myroi = { .x = 310,.y = 100,.w = 150,.h = 150 };
    CVIMAGE myImage1 = YMCV_Creat_ImgROICopyTo(myImage0, &myroi);

    //ֱ��ͼ����
    int32 myImage1Hist[256];

    /*--------------------------�Ҷ�ͼֱ��ͼ�����뷴��ӳ��--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 256, PIXChannelNum1);
    //��ʾimage1����ֱ��ͼ
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(500, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 550, 420, 128);

    //��Ϣ��ʾ
    sprintf(myInform, "��ȡ�����Ҷ�ֱ��ͼ");
    LCD_ShowString(100, 510, myInform);

    PAUSE();
    //image1ֱ��ͼ��ͶӰ��image0 ����ʾ
    YMCV_GrayHist_BackProject(myImage1Hist, 256, myImage0);
    LCD_show_image(500, 0, myImage0);

    //��Ϣ��ʾ
    sprintf(myInform, "�Ҷ�ֱ��ͼ����ͶӰ");
    LCD_ShowString(100, 510, myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





