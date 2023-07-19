#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ����ͼ��ֵ��Ϊ����չʾ�˻Ҷ���ֵ�ָ�Ч��
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


    /*--------------------------�ҶȻ���ͼ��ֵ��--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CV_binary_param myParam = { .gray_integral_t.kernerl_radius = 10 ,.gray_integral_t.thresh_percent = 85 };

    for (int i = 1; i < 10; i++)
    {

        //���ò���
        myParam.gray_integral_t.kernerl_radius = 5 + 2 * i;//�뾶
        myParam.gray_integral_t.thresh_percent = 85;//85%


        //������ֵ�任��Ⱦ��,����ͼ��ֵ��
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_IntegralThreshold);

        //���ز���������Ⱦ���á���ʼ��
        YMCV_BinaryKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

        LCD_Clear(0);
        //��ʾͼƬ0
        LCD_show_image(0, 0, myImage0);
        //��ʾͼƬ2
        LCD_show_image(500, 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "��ֵͼ�� ����ͼֱ�� %d ��ֵ %d %% ", myParam.gray_integral_t.kernerl_radius,
                                                                myParam.gray_integral_t.thresh_percent);
        LCD_ShowString(100, 310, myInform);
        //��ͣ
        PAUSE();

        //�ͷ���Ⱦ���������´��õ�ʱ�����ڴ�й©
        YMCV_Free_Binary_RenderFree(myRender1);

    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





