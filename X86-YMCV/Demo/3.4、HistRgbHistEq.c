#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼ���⻯��ֱ��ͼ�涨��Ϊ����չʾ��ֱ��ͼ��ʹ�ü���ʾ
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//��ɫͼ0

    uint8* myData1 = CV_malloc1(sizeof(linglong01));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData1, linglong01, sizeof(linglong01));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData1, 500, 255, CVRgb16Type);//��ɫͼ1

    //ֱ��ͼ����
    int32 myImage0Hist[3*256], myImage1Hist[3*256];

    /*--------------------------��ɫͼֱ��ͼ���⻯--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 3 * 256, PIXChannelNum3);
    //��ʾͼƬ��ֱ��ͼ
    LCD_show_image(0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum3, 50, 420, 128);

    //��ͼƬ0����һ��
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage0);
    //��myImage2�Ľ���ֱ��ͼ���⻯����
    YMCV_Image_Hist_Equalize(myImage2, myImage2);//�����������

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3 * 256, PIXChannelNum3);
    LCD_show_image(500, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 550, 420, 128);//���ڴ�С������128������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ

    sprintf(myInform, "��ͨ������ʽ���⻯");
    LCD_ShowString(300, 300, myInform);
    PAUSE();

    //��myImage0�Ľ���ֱ��ͼ���⻯����
    YMCV_Image_Hist_Equalize_UGray(myImage0, myImage2);

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3 * 256, PIXChannelNum3);
    LCD_show_image(500, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 550, 420, 128);//���ڴ�С������128������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ

    sprintf(myInform, "��ͨ���ϲ�ʽ���⻯");
    LCD_ShowString(300, 300, myInform);
    PAUSE();


    /*--------------------------��ɫͼֱ��ͼ�涨��--------------------------------------------------*/

    ////�ֶ���ֱ��ͼ�涨Ϊĳ����״
    //for (int i = 0; i < 256; i++)
    //{
    //    myImage1Hist[i] = i + 50;//���Ժ�������ֱ��ͼΪ���ν���
    //}
    //��ͼ��1��ȡֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 3*256, PIXChannelNum3);
    LCD_show_image(500, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum3,550, 420, 128);

    sprintf(myInform, "Ŀ��ͼ��ֱ��ͼչʾ");
    LCD_ShowString(300, 300, myInform);
    PAUSE();

    //��ͼ��0����ֱ��ͼ�涨��
    YMCV_Image_Hist_To(myImage0, myImage2, myImage1Hist, 3*256, PIXChannelNum3);
    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3*256, PIXChannelNum3);
    LCD_show_image(500, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3,550, 420, 128);//���ڴ�С������128������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ

    sprintf(myInform, "ԭʼͼ��涨��չʾ");
    LCD_ShowString(300, 300, myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





