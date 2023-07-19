#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * ˵��  ��������expָ���任�����ԭͼ���нṹ���Ƽ��㣬�ṹ�����ı����Ȳ����ı�ͼ��ṹ
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData2 = CV_malloc1(sizeof(tuxin_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData2, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage_0 = YMCV_Creat_DataToImg((uint8*)myData2, 250, 140, CVGrayType);//�Ҷ�ͼ

    /*--------------------------expָ����ǿ--------------------------------------------------*/

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);


    //���´���һ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//ָ����ǿ
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };

    YMCV_SingleChannel_MapTo(myRender, &myParam1);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
    YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);

    //��ʾͨ��ת��ͼ
    LCDChannelTransformShow(myRender, 540, 60, 40);

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(250, 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "expֵ����ǿ: a=%03.2f b= %01.3f c=%01.2f .", myParam1.gray_exp_enhance._a, myParam1.gray_exp_enhance._b, myParam1.gray_exp_enhance._c);
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------�ṹ���ƶȼ���--------------------------------------------------*/
    float similar = YMCV_Image_Gray_SSIM_Cal(myImage0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "     ����ͼ�����ƶ�Ϊ      %f %%.             ", similar * 100);
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();

    /*--------------------------�ṹ���ƶȼ���--------------------------------------------------*/
    similar = YMCV_Image_Gray_SSIM_Cal(myImage0, myImage_0);

    LCD_show_image(250, 0, myImage_0);
    //��Ϣ��ʾ
    sprintf(myInform, "     ����ͼ�����ƶ�Ϊ      %f %%.             ", similar * 100);
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage_0);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





