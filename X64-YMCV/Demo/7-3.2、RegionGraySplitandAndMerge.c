#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"

#include"../YMCV/CORE/YMCV_SegnmCal.h"

/*******************************************************************************************
    *
    * ˵��  �������� ���� �ϲ�Ϊ����չʾ��һ��ͼ��ֱ���з�����ϲ��Ĵ���Ч��
    *           ��ʵ�������������Ǵ���ʹ�ã������ȶ�ͼ�����������ѣ�Ȼ��Խ��ͼ��������ϲ����Ӷ��õ��ȽϹ�����ͼ��
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    LCD_Clear(0);
    //��ʾͼƬ0
    LCD_show_image(0, 0, myImage0);


    /*--------------------------�Ҷ� �������--------------------------------------------------*/
    for (int i = 0; i < 8; i++)
    {
        int thresh = i * 20;

        CVIMAGE myImage2 = YMCV_Gray_Region_Splitand(myImage0, thresh);//����

        //��ʾͼƬ2
        LCD_show_image(500, 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "���������ֵ��%d ",thresh);
        LCD_ShowString(100, 310, myInform);
        //��ͣ
        PAUSE();

        //�ͷ��ڴ�
        YMCV_Free_ImgFree(myImage2);
    }
   

    /*--------------------------�Ҷ� ����ϲ�--------------------------------------------------*/
    for (int i = 0; i < 8; i++)
    {
        int thresh = i;

        CVIMAGE myImage2 = YMCV_Gray_Region_Merge(myImage0, thresh);//�ϲ�

        //��ʾͼƬ2
        LCD_show_image(500, 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "����ϲ���ֵ��%d     ", thresh);
        LCD_ShowString(100, 310, myInform);
        //��ͣ
        PAUSE();

        //�ͷ��ڴ�
        YMCV_Free_ImgFree(myImage2);
    }


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}









