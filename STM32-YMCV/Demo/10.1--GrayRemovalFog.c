#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  ������ ��ͨ��ȥ��ʵ�� ����Ч��
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = (uint8*)CV_malloc1(sizeof(wutu2_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)wutu2_gray, sizeof(wutu2_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg(myData, 160, 120, CVGrayType);//�Ҷ�ͼ
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;

    //�����ռ���Ⱦ��
    uint8 kernel_r = 1;
    YMCV_PADDING_VAL = 125;//�������ֵ

    //���ڻ�����
    CVIMAGE myImage1 = NULL, myImage2 = NULL;

    //��ð�ͨ��ͼ
    myImage1 = YMCV_Gray_Image_GetDarkChannel(myImage0_gray, kernel_r);
    //�����������A
    uint8 AtmosValue = YMCV_Gray_Image_GetAtmosphericlighting(myImage0_gray, myImage1);

    /*--------------------------��ͨ��ȥ�� �޸��˲��뾶--------------------------------------------------*/
    for (float32 k = 0.1; k < 1.0; k+=0.1)
    {
        //���ó����� k���õ�ȥ��ͼ��
        myImage2 = YMCV_Gray_Image_RemovalFog(myImage0_gray, myImage1, AtmosValue,k);
        //������ԵĻ�����ʹ��ͨ����Ⱦ������һ��ͼ����������ȣ�Ч����ȽϺ�
        
        //������ʾͼƬ2
        LCD_show_image(stx + 0, sty + 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "fog remove: %.2f  ", k);
        LCD_ShowString(stx + 0, sty + 0, myInform);
        //��ͣ
        Delay(300);

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
}








