#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� FishEyeΪ����չʾ������ģ�⼰����Ч����������ͼΪ�����ν���Ч���ź�
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

    LCD_Clear(GRAY);
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;
    /*-------------------------- ����Ч�� --------------------------------------------------*/
   
    CVIMAGE feImg = YMCV_Gray_FishEye_Effect(myImage0_gray, myImage0_gray->width, myImage0_gray->height);
    LCD_show_image(stx + 0, sty, feImg);
    //��Ϣ��ʾ
    sprintf(myInform, "FishEye Effect ");
    LCD_ShowString(stx + 0, sty, myInform);
    Delay(300);
    /*-------------------------- ͸��ʧ����� --------------------------------------------------*/
    YMCV_PADDING_VAL = 50;//�հ״����ֵ
    CVIMAGE invfeImg = YMCV_Gray_invFishEye_Effect(feImg,4, 0.43, 0.85);
    LCD_show_image(stx + 0, sty, invfeImg);
    //��Ϣ��ʾ
    sprintf(myInform, "inverse FishEye Effect ");
    LCD_ShowString(stx + 0, sty, myInform);
    Delay(300);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(feImg);
    YMCV_Free_ImgFree(invfeImg);
    YMCV_Free_ImgFree(myImage0_gray);
}

