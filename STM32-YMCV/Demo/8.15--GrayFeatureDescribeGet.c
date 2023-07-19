#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ���������� Ϊ����չʾ��brief������������ȡ 
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
    /*------------------------ AGAST ��������ȡ ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//��ֵ
    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//��������

    /*------------------------ rotate brief������������ȡ ------------------------------*/
    CVVECTORS_U16 featdesc = YMCV_FeatureDescribe_RotateBrief_Cal(myImage0_gray, featpoints);


    LCD_show_image(stx + 0, sty + 0, myImage0_gray);//��ʾͼƬ
    //��ʾ����������
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//������
        CV_featdesc_type* desc = (CV_featdesc_type*)(featdesc->datap + i * featdesc->dim);//��������

        //�������ķ���ֱ��
        int r = 15;//����ģ��ѡ��r=15������������������ֱ���û���r = 15��Բ��
        int lendx = r * YMCV_Cos(desc->angle * YMCV_Deg2Rad);
        int lendy = r * YMCV_Sin(desc->angle * YMCV_Deg2Rad);
        color24_t thiscolor = { .R = 255 };
        LCD_Point_Color_Set(thiscolor);
        LCD_Draw_Line(stx + pp->x, sty + pp->y, stx + pp->x + lendx, sty + pp->y + lendy);
        //���Ƶ�λ��
        color24_t thiscolor2 = { .G = 255 };
        LCD_Point_Color_Set(thiscolor2);
        LCD_Draw_Circle(stx + pp->x, sty + pp->y, r);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "AGAST And Rotate BRIEF");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);
		
    //�ͷ�featpoints�ڴ�
    YMCV_Free_Vector_U16_Free(featdesc);
    YMCV_Free_Vector_U16_Free(featpoints);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    //YMCV_Free_ImgFree(myImage1);

    Delay(500);
}





