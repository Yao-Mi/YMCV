#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ���߼�����Ϊ����չʾ���߼� ���� �� ���
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
    CV_binary_param myParam = { .gray_single_t.thresh = 120 };
    //������ֵ�任��Ⱦ��
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_SingleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //���ز���������Ⱦ���á���ʼ��

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

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

    /*-------------------------- ��ֵ���� myImage3 --------------------------------------------------*/
    //�����µĲ���
    myParam.gray_double_t.thresh_min = 50;
    myParam.gray_double_t.thresh_max = 150;
    //�����µĶ�ֵ�任��Ⱦ��
    myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //���ز���������Ⱦ���á���ʼ��

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(myImage0, myImage3, myRender1);

    //��ʾͼƬ3
    LCD_show_image(500, 0, myImage3);

    //��Ϣ��ʾ
    sprintf(myInform, "��ֵͼ��3 ��ֵ: %d ��%d", myParam.gray_double_t.thresh_min,myParam.gray_double_t.thresh_max);
    LCD_ShowString(600, 310, myInform);
    //��ͣ
    PAUSE();
    

    /*-------------------------- myImage3 = myImage3 & myImage2 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(myImage2, myImage3, CV_And);//ͼ2 �߼����� ͼ3 ������������ͼ3

    //��ʾͼƬ3
    LCD_show_image(250, 150, myImage3);
    //��Ϣ��ʾ
    sprintf(myInform, "��ֵͼ�� ͼ2 & ͼ3 ");
    LCD_ShowString(300, 310, myInform);
    //��ͣ
    PAUSE();

    LCD_show_image(500, 0, myImage3);//����ͼ3
    /*-------------------------- myImage3 = myImage2 | myImage3 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(myImage2, myImage3, CV_Or);//ͼ2 �߼����� ͼ3 ������������ͼ3

    //��ʾͼƬ3
    LCD_show_image(250, 150, myImage3);
    //��Ϣ��ʾ
    sprintf(myInform, "��ֵͼ�� ͼ2 | ͼ3 ");
    LCD_ShowString(300, 310, myInform);
    //��ͣ
    PAUSE();
    /*-------------------------- myImage3 = ! myImage2 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(myImage2, myImage3, CV_Not);//ͼ2 �߼���  ������������ͼ3

    //��ʾͼƬ3
    LCD_show_image(250, 150, myImage3);
    //��Ϣ��ʾ
    sprintf(myInform, "��ֵͼ�� ��ͼ2     ");
    LCD_ShowString(300, 310, myInform);
    //��ͣ
    PAUSE();


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





