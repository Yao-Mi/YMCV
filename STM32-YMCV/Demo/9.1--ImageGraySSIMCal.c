#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  ��������expָ���任�����ԭͼ���нṹ���Ƽ��㣬�ṹ�����ı����Ȳ����ı�ͼ��ṹ
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

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData2 = CV_malloc1(sizeof(tuxin02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData2, (void*)tuxin02_gray, sizeof(tuxin02_gray));
    CVIMAGE myImage1_gray = YMCV_Creat_DataToImg((uint8*)myData2, 160, 120, CVGrayType);//�Ҷ�ͼ

    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;
    /*--------------------------expָ����ǿ--------------------------------------------------*/

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage_0 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, myImage0_gray->type);


    //���´���һ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//ָ����ǿ
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.17f };

    YMCV_SingleChannel_MapTo(myRender, &myParam1);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
    YMCV_Image_ChannelRenderTo(myImage0_gray, myImage_0, myRender);

    //��ʾͼƬ
    LCD_show_image(stx + 0, sty, myImage_0);
    //��ʾͨ��ת��ͼ
    LCDChannelTransformShow(myRender, 170, 220, 40);

    //��Ϣ��ʾ
    sprintf(myInform, "exp:a%03.2f b%01.3f c%01.2f", myParam1.gray_exp_enhance._a, myParam1.gray_exp_enhance._b, myParam1.gray_exp_enhance._c);
    LCD_ShowString(0, sty, myInform);
    //��ͣ
    Delay(500);
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------�ṹ���ƶȼ���--------------------------------------------------*/
    float similar = YMCV_Image_Gray_SSIM_Cal(myImage0_gray, myImage_0);

    //��ʾͼƬ
    LCD_show_image(stx + 0, sty, myImage_0);
    //��Ϣ��ʾ
    sprintf(myInform, "similar = %f %%    ", similar * 100);
    LCD_ShowString(0, sty, myInform);
    //��ͣ
    Delay(500);

    /*--------------------------�ṹ���ƶȼ���--------------------------------------------------*/
    similar = YMCV_Image_Gray_SSIM_Cal(myImage0_gray, myImage1_gray);

    LCD_show_image(stx + 0, sty, myImage1_gray);
    //��Ϣ��ʾ
    sprintf(myInform, "similar = %f %%    ", similar * 100);
    LCD_ShowString(0, sty, myInform);
    //��ͣ
    Delay(500);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1_gray);
    YMCV_Free_ImgFree(myImage_0);

}





