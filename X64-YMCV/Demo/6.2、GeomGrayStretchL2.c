#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_GeomCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ͼ�������任Ϊ����չʾ�����Բ�ֵ��Ч��
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

    uint8* myData1 = CV_malloc1(sizeof(xgmm_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData1, xgmm_gray, sizeof(xgmm_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData1, 50, 75, CVGrayType);//�Ҷ�ͼ1


    /*--------------------------˫���Բ�ֵ--------------------------------------------------*/
    CVIMAGE myImage2 = NULL;
    CV_geom_param myParam = { .stretch.Imgin_w = myImage0->width,
                              .stretch.Imgin_h = myImage0->height,
                              .stretch.out_w = myImage0->width,
                              .stretch.out_h = myImage0->height, };
    float k = 1.0;
    for (int i = 1; i < 10; i++)
    {
        //��ʼ������
        myParam.stretch.out_w = (int)(k * myImage0->width);
        myParam.stretch.out_h = (int)(k * myImage0->height);

        //�������α任��Ⱦ��
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);

        //ʹ�ò������м�����Ⱦ���á���ʼ��
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);

        LCD_Clear(0);
        //��ʾͼƬ0
        LCD_show_image(0, 0, myImage0);
        //��ʾͼƬ2
        LCD_show_image(500, 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "˫���Բ�ֵ����ͼ�� k: %02.1f   . ", k);
        LCD_ShowString(100, 310, myInform);
        //��ͣ
        PAUSE();


        if (i < 4)
        {
            k -= 0.2;
        }
        else
        {
            k += 0.2;
        }
        //�ͷ���Ⱦ��
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
    }

    //ͨ�ò�����ʼ��
    myParam.stretch.Imgin_w = myImage1->width;
    myParam.stretch.Imgin_h = myImage1->height;
    k = 1.0;
    for (int i = 1; i < 10; i++)
    {
        //��ʼ������
        myParam.stretch.out_w = (int)(k * myImage1->width);
        myParam.stretch.out_h = (int)(k * myImage1->height);

        //�������α任��Ⱦ��
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);

        //ʹ�ò������м�����Ⱦ���á���ʼ��
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        myImage2 = YMCV_Image_Geom_RenderTo(myImage1, myRender1);


        LCD_Clear(0);
        //��ʾͼƬ0
        LCD_show_image(0, 0, myImage1);
        //��ʾͼƬ2
        LCD_show_image(500, 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "˫���Բ�ֵ����ͼ�� k: %02.2f   . ", k);
        LCD_ShowString(100, 310, myInform);
        //��ͣ
        PAUSE();

        if (i < 4)
        {
            k *= 0.8;
        }
        else
        {
            k *= 1.6;
        }

        //�ͷ���Ⱦ��
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
    }
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





