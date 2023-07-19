#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ������˹-��˹�˲���Ϊ����չʾ�˱�Ե���Ч��
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


    /*--------------------------������˹ - ��˹ ��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CV_edge_param myParam = { .EdgeType = CV_Log ,.Thresh.log.minT=60};

    YMCV_Gray_EdgeDetect(myImage0, myImage2, &myParam);

    LCD_Clear(0);
    //��ʾͼƬ0
    LCD_show_image(0, 0, myImage0);
    //��ʾͼƬ2
    LCD_show_image(500, 0, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "Log���ͼ�� ����ֵ��%d", myParam.Thresh.log.minT);
    LCD_ShowString(100, 310, myInform);
    //��ͣ
    PAUSE();

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





