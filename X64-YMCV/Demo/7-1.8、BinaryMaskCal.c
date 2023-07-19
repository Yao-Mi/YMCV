#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ����������Ϊ����չʾ�����뱣�� �� ���
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


    //����������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ


    /*-------------------------- ��ֵ���� myImage2 --------------------------------------------------*/
    CV_binary_param myParam = { .gray_single_t.thresh = 210 };
    //������ֵ�任��Ⱦ��
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_SingleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //���ز���������Ⱦ���á���ʼ��

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

    //���ٷ�תһ��
    for (int i = 0; i < myImage2->width* myImage2->height; i++)
    {
        myImage2->data[i] = 255 - myImage2->data[i];
    }
    //�ͷ���Ⱦ���ڴ�
    YMCV_Free_Binary_RenderFree(myRender1);

    //��ʾͼƬ0
    LCD_Clear(0);
    LCD_show_image(500, 0, myImage0);
    LCD_show_image(0, 0, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "��ֵͼ��2 ��ֵ: %d  ", myParam.gray_single_t.thresh);
    LCD_ShowString(100, 310, myInform);
    //��ͣ
    PAUSE();


    /*-------------------------- ���뱣�� myImage0 = Mask & myImage0 --------------------------------------------------*/
    YMCV_Image_BinaryMask_Cal(myImage0, myImage2);//���뱣��������������ͼ0

    //��ʾͼƬ3
    LCD_show_image(250, 150, myImage0);
    //��Ϣ��ʾ
    sprintf(myInform, "   ���뱣��          ");
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






