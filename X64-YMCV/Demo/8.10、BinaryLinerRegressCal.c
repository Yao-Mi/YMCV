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
    * ˵��  �������� ��ֵͼ���ػع�Ϊ����չʾ�����Իع�Ч����
    *  �÷���ͨ����ͼ���ز���Ϊ������25x25��С��ͼ�񣬽��лع�������ٽ���任��ԭͼ�����ܻ������ԭͼʵ��ֱ�߲���ȫ�غϵ����
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(xunxian02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, xunxian02_gray, sizeof(xunxian02_gray));
    
    for (int i = 0; i < sizeof(xunxian02_gray); i++)
    {
        myData[i] = (myData[i] < 127) ? 255 : 0; //���ٶ�ֵ��
    }
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVBinaryType);//�Ҷ�ͼ0

    /*-------------------------��С���� ֱ�߻ع����--------------------------------------------------*/
    CVline imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0, CVRegress_LSE);
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//��ʾͼƬ0
    LCD_Draw_Line(imgline.x1, imgline.y1, imgline.x2, imgline.y2);//����ֱ��

    //��Ϣ��ʾ
    sprintf(myInform, "��С���� ֱ�߻ع� ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();


    /*-------------------------̩��ɭ ֱ�߻ع����--------------------------------------------------*/
    imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0, CVRegress_TSE);
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//��ʾͼƬ0
    LCD_Draw_Line(imgline.x1, imgline.y1, imgline.x2, imgline.y2);//����ֱ��
    //��Ϣ��ʾ
    sprintf(myInform, "̩��ɭ ֱ�߻ع� ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();


    /*-------------------------RANSAC ֱ�߻ع����--------------------------------------------------*/
    imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0, CVRegress_RANSAC);
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//��ʾͼƬ0
    LCD_Draw_Line(imgline.x1, imgline.y1, imgline.x2, imgline.y2);//����ֱ��
    //��Ϣ��ʾ
    sprintf(myInform, "RANSAC ֱ�߻ع� ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





