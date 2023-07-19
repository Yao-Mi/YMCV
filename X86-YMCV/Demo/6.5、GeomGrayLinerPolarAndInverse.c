#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_GeomCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ͼ������任Ϊ����չʾ��ͼƬ���Լ������µ�ͶӰЧ��
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

    /*--------------------------ͶӰ����������--------------------------------------------------*/
    CVIMAGE myImage2 = NULL, myImage3 = NULL;
    YMCV_PADDING_VAL = 50;//�հ������ֵ

    //����ͼƬ0����Ⱦ����
    CV_geom_param myParam = { .polar.Imgin_w = myImage0->width,
                              .polar.Imgin_h = myImage0->height };
    //�������α任��Ⱦ��
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_LinerPolar);

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
    sprintf(myInform, "ͼ�� ���Լ�����任  . ");
    LCD_ShowString(100, 310, myInform);
    //��ͣ
    PAUSE();

    /*--------------------------ͶӰ�صѿ�������ϵ--------------------------------------------------*/
    //����ͼƬ2����Ⱦ����
    myParam.rotate.Imgin_w = myImage2->width;
    myParam.rotate.Imgin_h = myImage2->height;
    //�������α任��Ⱦ��
    CVGEOM_RENDER myRender2 = YMCV_Creat_GeomRender_Creat(CV_LinerPolar_Inverse);

    //ʹ�ò������м�����Ⱦ���á���ʼ��
    YMCV_GeomKernel_Load(myRender2, &myParam);

    //��Ŀ��ͼ��Ľ�����Ⱦ
    myImage3 = YMCV_Image_Geom_RenderTo(myImage2, myRender2);
    //���ͼ���Ȼ�����������У���Ϊ������һ�����ĵ�

    LCD_Clear(0);
    //��ʾͼƬ0
    LCD_show_image(0, 0, myImage2);
    //��ʾͼƬ2
    LCD_show_image(500, 0, myImage3);


    //��Ϣ��ʾ
    sprintf(myInform, "ͼ�� ���Լ�������任  . ");
    LCD_ShowString(200, 310, myInform);

    //�ͷ���Ⱦ��
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_GEOM_RenderFree(myRender2);
    //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





