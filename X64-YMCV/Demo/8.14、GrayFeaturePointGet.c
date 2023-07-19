#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� �߽���� Ϊ����չʾ�˱߽�������ȡ�Լ���״�б�
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    /*------------------------ SUASAN��������ȡ��ȡ ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_SUSAN;
    myparam.param.susan.thresh = 20;

    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//��������
    
    LCD_show_image(300, 0, myImage0);
    //��ʾ������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(300 + pp->x - 1, pp->y - 1, 2, 2, thiscolor);
        //LCD_Draw_Point(300+pp->x, pp->y, thiscolor);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "SUSAN��������ȡ  ");
    LCD_ShowString(400, 310, myInform);

    //�ͷ�featpoints�ڴ�
    YMCV_Free_Vector_U16_Free(featpoints);
    PAUSE();

    /*------------------------ Fasts��������ȡ ------------------------------*/
    myparam.type = CV_FeatPoint_Fast;
    myparam.param.fast.thresh = 20;//��ֵ

    featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//��������

    LCD_show_image(300, 0, myImage0);
    //��ʾ������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(300 + pp->x - 1, pp->y - 1, 2, 2, thiscolor);
    }

    
    //��Ϣ��ʾ
    sprintf(myInform, "Fast9��������ȡ  ");
    LCD_ShowString(400, 310, myInform);

    //�ڴ��ͷ�
    YMCV_Free_Vector_U16_Free(featpoints);
    PAUSE();

    /*------------------------ AGAST ��������ȡ ------------------------------*/
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//��ֵ

    featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//��������

    LCD_show_image(300, 0, myImage0);
    //��ʾ������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(300 + pp->x - 1, pp->y - 1, 2, 2, thiscolor);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "AGAST ��������ȡ  ");
    LCD_ShowString(400, 310, myInform);

    //�ڴ��ͷ�
    YMCV_Free_Vector_U16_Free(featpoints);
    PAUSE();

    /*------------------------ Harris ��������ȡ ------------------------------*/
    myparam.type = CV_FeatPoint_Harris;
    myparam.param.harris.thresh = 5;//�޳��ٷֱ� 5/255

    featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//��������

    LCD_show_image(300, 0, myImage0);
    //��ʾ������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(300 + pp->x - 1, pp->y - 1, 2, 2, thiscolor);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "Harris ��������ȡ  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    //�ͷ�featpoints�ڴ�
    YMCV_Free_Vector_U16_Free(featpoints);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    //YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





