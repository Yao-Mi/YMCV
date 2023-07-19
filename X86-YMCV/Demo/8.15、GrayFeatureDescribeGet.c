#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ���������� Ϊ����չʾ��brief������������ȡ 
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    /*------------------------ AGAST ��������ȡ ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//��ֵ
    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//��������

    LCD_show_image(0, 0, myImage0);//��ʾͼƬ

    /*------------------------ rotate brief������������ȡ ------------------------------*/
    CVVECTORS_U16 featdesc = YMCV_FeatureDescribe_RotateBrief_Cal(myImage0, featpoints);


    LCD_show_image(0, 0, myImage0);//��ʾͼƬ
    //��ʾ����������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//������
        CV_featdesc_type* desc = featdesc->datap + i * featdesc->dim;//��������

        //�������ķ���ֱ��
        int r = 15;//����ģ��ѡ��r=15������������������ֱ���û���r = 15��Բ��
        int lendx = r * YMCV_Cos(desc->angle * YMCV_Deg2Rad);
        int lendy = r * YMCV_Sin(desc->angle * YMCV_Deg2Rad);
        color24_t thiscolor = { .R = 255 };
        LCD_Point_Color_Set(thiscolor);
        LCD_Draw_Line(pp->x, pp->y, pp->x + lendx, pp->y + lendy);
        //���Ƶ�λ��
        color24_t thiscolor2 = { .G = 255 };
        LCD_Point_Color_Set(thiscolor2);
        LCD_Draw_Circle(pp->x, pp->y, r);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "AGAST ��������RotateBRIEF��������ȡ  ");
    LCD_ShowString(200, 310, myInform);
    PAUSE();
    //�ͷ�featpoints�ڴ�
    YMCV_Free_Vector_U16_Free(featdesc);
    YMCV_Free_Vector_U16_Free(featpoints);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    //YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





