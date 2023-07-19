#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ������ Ϊ����չʾ��susan��fast-9��agast��harris��������ȡ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty =  myImage0_gray->height;
    /*------------------------ SUASAN��������ȡ��ȡ ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_SUSAN;
    myparam.param.susan.thresh = 20;

    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//��������
    
    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //��ʾ������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(stx + pp->x - 1, sty + pp->y - 1, 2, 2, thiscolor);
        //LCD_Draw_Point(stx+pp->x, sty + pp->y, thiscolor);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "SUSAN FeatPoint ");
    LCD_ShowString(stx + 0, sty + 0, myInform);

    //�ͷ�featpoints�ڴ�
    YMCV_Free_Vector_U16_Free(featpoints);
    Delay(500);

    /*------------------------ Fasts��������ȡ ------------------------------*/
    myparam.type = CV_FeatPoint_Fast;
    myparam.param.fast.thresh = 20;//��ֵ

    featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//��������

    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //��ʾ������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(stx + pp->x - 1, sty + pp->y - 1, 2, 2, thiscolor);
    }

    
    //��Ϣ��ʾ
    sprintf(myInform, "Fast9 FeatPoint  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);

    //�ڴ��ͷ�
    YMCV_Free_Vector_U16_Free(featpoints);
    Delay(500);

    /*------------------------ AGAST ��������ȡ ------------------------------*/
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//��ֵ

    featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//��������

    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //��ʾ������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(stx + pp->x - 1, sty + pp->y - 1, 2, 2, thiscolor);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "AGAST FeatPoint  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);

    //�ڴ��ͷ�
    YMCV_Free_Vector_U16_Free(featpoints);
    Delay(500);

    /*------------------------ Harris ��������ȡ ------------------------------*/
    myparam.type = CV_FeatPoint_Harris;
    myparam.param.harris.thresh = 5;//�޳��ٷֱ� 5/255

    featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//��������

    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //��ʾ������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(stx + pp->x - 1, sty + pp->y - 1, 2, 2, thiscolor);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "Harris FeatPoint  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);

    //�ͷ�featpoints�ڴ�
    YMCV_Free_Vector_U16_Free(featpoints);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    //YMCV_Free_ImgFree(myImage1);
}





