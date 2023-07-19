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
    * ˵��  �������� ��������Ϊ����չʾ��mask��ֵͼ����
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

    /*--------------------------�Ҷ���������--------------------------------------------------*/
    int x = 80, y = 80;

    for (int i = 0; i < 10; i++)
    {
        int thresh = 30;
        //�������� ����mask��ֵͼ
        CVIMAGE myImage2 = YMCV_Gray_Region_Growing8(myImage0, x, y, thresh);

        LCD_Clear(0);
        //��ʾͼƬ0
        LCD_show_image(0, 0, myImage0);
        //�������ӵ�λ��
        color24_t cvl = { .R = 255,.G = 120,.B = 50 };
        LCD_Fill_ConstRgbRect(x - 1, y - 1, 3, 3, cvl);
        //��ʾͼƬ2
        LCD_show_image(500, 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "�������� �������� %d %d, ������ֵ��%d",x,y, thresh);
        LCD_ShowString(100, 310, myInform);
        //��ͣ
        PAUSE();
    
        x += 8;//�ƶ����ӵ�

        //�ͷ�ͼ���ڴ�
        YMCV_Free_ImgFree(myImage2);
    }

 

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}









