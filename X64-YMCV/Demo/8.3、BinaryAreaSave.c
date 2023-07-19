#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_PixConv.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� ������Ϊ����չʾ�� �����ϴ���������Ч��
    *
    ***************************************************************************************/
int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0


    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);
    /*--------------------------�Ҷ�˫��ֵ��ֵ��--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CV_binary_param myParam = { .gray_double_t.thresh_min = 100 ,.gray_double_t.thresh_max = 150 };

    //������ֵ�任��Ⱦ��
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);//���ز���������Ⱦ���á���ʼ��
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ
    
    //��ʾԭͼ
    LCD_show_image(300, 0, myImage2);
    PAUSE();
    /*------------------------ ��ֵ������� ------------------------------*/
    //�����������100������
    YMCV_Binary_AreaFilter(myImage2, 100);
    LCD_show_image(300, 0, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "����ͼ  ");
    LCD_ShowString(400, 310, myInform);
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Binary_RenderFree(myRender1);

    PAUSE();
    LCD_Destory();
    return 0;
}

