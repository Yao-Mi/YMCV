#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ģ��ƥ��Ϊ����չʾ���ַ���ƥ��Ľ��
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

    /*--------------------------expָ����ǿ--------------------------------------------------*/
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, myImage0_gray->type);

    //���´���һ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//ָ����ǿ
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };

    YMCV_SingleChannel_MapTo(myRender, &myParam1);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
    YMCV_Image_ChannelRenderTo(myImage0_gray, myImage1, myRender);

    //��ȡ����ͼ��
    CVIMAGE imgFind = YMCV_Creat_ImgCopyTo(myImage1, 35, 35, 30, 30);

    //��ʾͼƬ
		LCD_Clear(WHITE);
    LCD_show_image(0, 0, myImage0_gray);
    LCD_show_image(myImage0_gray->width + 5, 16, imgFind);

    //��Ϣ��ʾ
    sprintf(myInform, "exp temp");
    LCD_ShowString(myImage0_gray->width + 5, 0, myInform);
    //��ͣ
    Delay(300);
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);
    /*-------------------------- MAD ģ��ƥ�����--------------------------------------------------*/

    //ƽ�����Բ 
    CVrect result = YMCV_Gray_Matching_Template(myImage0_gray, imgFind, CV_Template_MAD, 4, 30); //�����صĿ����0����˵��������

    //��ʾ���
    color24_t rgbval = { .R = 255 };
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //��Ϣ��ʾ
    sprintf(myInform, "  MAD   ");
    LCD_ShowString(0, 0, myInform);
    //��ͣ
    Delay(500);

    /*-------------------------- SSDA ģ��ƥ�����--------------------------------------------------*/

    //ƽ�����Բ 
    result = YMCV_Gray_Matching_Template(myImage0_gray, imgFind, CV_Template_SSDA, 4, 30); //�����صĿ����0����˵��������

    //��ʾ���
    LCD_show_image(0, 0, myImage0_gray);
    rgbval.G = 255;
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //��Ϣ��ʾ
    sprintf(myInform, "  SSDA  ");
    LCD_ShowString(0, 0, myInform);
    //��ͣ
    Delay(500);

    /*-------------------------- NCC ģ��ƥ�����--------------------------------------------------*/
    //ƽ�����Բ 
    result = YMCV_Gray_Matching_Template(myImage0_gray, imgFind, CV_Template_NCC, 4, 30); //�����صĿ����0����˵��������

    //��ʾ���
    LCD_show_image(0, 0, myImage0_gray);
    rgbval.R = 0;
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //��Ϣ��ʾ
    sprintf(myInform, "  NCC   ");
    LCD_ShowString(0, 0, myInform);
    //��ͣ
    Delay(500);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(imgFind);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
}





