#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� inpaintΪ����չʾ���Զ��޸���Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(house2_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)house2_gray, sizeof(house2_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------- �ֶ�����maskͼ--------------------------------------------------*/
    LCD_Clear(GRAY);
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage0_gray);
    int sty = myImage0_gray->height;
	  Delay(300);
    color24_t thiscolor = { .R = 255,.G = 0 };
    LCD_Point_Color_Set(thiscolor);

    uint16 xpoint[] = { 135, 150, 150, 135, };
    uint16 ypoint[] = { 55, 55, 70, 70, };
    uint16 num = sizeof(xpoint) / 2;
    //����������������ο�PC�汾��Demo
    int i = 0;
    //��������
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(stx + xpoint[i], ypoint[i], stx + xpoint[i + 1], ypoint[i + 1]);
    }
    LCD_Draw_Line(stx + xpoint[i], ypoint[i], stx + xpoint[0], ypoint[0]);

    //����Ϊ�㼯
    CVpoint* myPoint = (CVpoint*)CV_malloc1(num * sizeof(CVpoint));//�㼯
    for (int i = 0; i < num; i++)
    {
        CVpoint* thiscolor = myPoint + i;
        thiscolor->x = xpoint[i];
        thiscolor->y = ypoint[i];
    }

    CVIMAGE Masker = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);
    CVcolor fillval;
    fillval.gray = 0;
    YMCV_Image_FullFill(Masker, fillval);
    //������ָ�����������
    fillval.gray = 255;
    YMCV_Img_Scanline_AreaFill(Masker, myPoint, num, fillval);
    LCD_show_image(stx + 0, sty+0, Masker);

    YMCV_Image_MaskFill(myImage0_gray, Masker, fillval);//ԭͼ��Mask��ʾ
    LCD_show_image(stx + 0, 0, myImage0_gray);
    //��Ϣ��ʾ
    sprintf(myInform, "mask image ");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(300);
		
    //inpainting�޸�
    YMCV_Gray_Exemplar_Based_Inpainting(myImage0_gray, Masker,4,9);

    //��ʾͼƬ
    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //��Ϣ��ʾ
    sprintf(myInform, "Inpainting Result");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(Masker);
    YMCV_Free_ImgFree(myImage0_gray);

}

