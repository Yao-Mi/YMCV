#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ��ͨ��ЧΪ����չʾ�˿�ͨ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ

    LCD_Clear(GRAY);
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage0_gray);
    /*-------------------------- ��ͨЧ�� --------------------------------------------------*/

    CVIMAGE feImg = YMCV_Gray_Cartoon_Effect(myImage0_gray,1);
    LCD_show_image(stx, myImage0_gray->height, feImg);
    //��Ϣ��ʾ
    sprintf(myInform, "Cartoon Effect ");
    LCD_ShowString(stx, myImage0_gray->height, myInform);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(feImg);
    YMCV_Free_ImgFree(myImage0_gray);

    Delay(500);
}

