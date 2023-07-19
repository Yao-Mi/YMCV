#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� Sobel�˲���Ϊ����չʾ�˱�Ե���Ч��
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


    /*--------------------------�Ҷ�Sobel��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CV_edge_param myParam = { .EdgeType = CV_Sobel_Connect ,.Thresh.sobel_connect.minT = 100,
    .Thresh.sobel_connect.connect_r=2,//���ƴ���뾶
    .Thresh.sobel_connect.valSimilarT=30,//��������
    .Thresh.sobel_connect.angSimilarT=5};//�Ƕ�����

    YMCV_Gray_EdgeDetect(myImage0, myImage2, &myParam);

    LCD_Clear(0);
    //��ʾͼƬ0
    LCD_show_image(0, 0, myImage0);
    //��ʾͼƬ2
    LCD_show_image(500, 0, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "Sobel���ͼ�� ����ֵ��%d ���ư뾶%d �Ƕ�����%d ��������%d  ", myParam.Thresh.sobel_connect.minT,
        myParam.Thresh.sobel_connect.connect_r ,
        myParam.Thresh.sobel_connect.angSimilarT,
        myParam.Thresh.sobel_connect.valSimilarT);
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





