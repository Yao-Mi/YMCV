#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * ˵��   ��������gama��s-curve��ǿΪ����չʾ�˲���ȡֵ �������������Լ�ͨ��ת��ʾ��ͼ����ʾ
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//��ɫͼ

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);


    /*--------------------------��ɫͼ٤��������ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Gama_Enhance, PIXChannelNum3);//gama������ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     c��5��    ��  gama��0.1��
    CV_pixchannel_3param myParam = { .rgb_gama_enhance.R_c = 309.0f,.rgb_gama_enhance.R_gama = 1.6f ,
                                     .rgb_gama_enhance.B_c = 309.0f,.rgb_gama_enhance.B_gama = 1.6f ,
                                     .rgb_gama_enhance.G_c = 309.0f,.rgb_gama_enhance.G_gama = 1.6f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);
        //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 540, 70, 60);
        //��Ϣ��ʾ
        sprintf(myInform, "gama��ǿ R: c=%03.2f gama= %01.3f  .  ", myParam.rgb_gama_enhance.R_c, myParam.rgb_gama_enhance.R_gama);
        LCD_ShowString(100, 300, myInform);
        sprintf(myInform, "gama��ǿ G: c=%03.2f gama= %01.3f  .  ", myParam.rgb_gama_enhance.G_c, myParam.rgb_gama_enhance.G_gama);
        LCD_ShowString(100, 320, myInform);
        sprintf(myInform, "gama��ǿ B: c=%03.2f gama= %01.3f  .  ", myParam.rgb_gama_enhance.B_c, myParam.rgb_gama_enhance.B_gama);
        LCD_ShowString(100, 340, myInform);

        //��ͣ
        PAUSE();

        //��������
        myParam.rgb_gama_enhance.R_c += 20.0f;
        //myParam.gray_gama_enhance._gama += 0.1f;
        myParam.rgb_gama_enhance.G_c += 15.0f;
        myParam.rgb_gama_enhance.B_c += 10.0f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------��ɫͼS������ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Scurve_Enhance, PIXChannelNum3);//S������ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��10��    ��  b��0.001��    ��   c��0.01��
    CV_pixchannel_3param myParam1 = { .rgb_scurve_enhance.R_m = 132.0f,.rgb_scurve_enhance.R_E = -5.0f,
                                      .rgb_scurve_enhance.G_m = 132.0f,.rgb_scurve_enhance.G_E = -5.0f,
                                      .rgb_scurve_enhance.B_m = 132.0f,.rgb_scurve_enhance.B_E = -5.0f };
    for (int i = 0; i < 20; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 540, 70, 60);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "S������ǿ R: m = %03.2f  E = %02.2f .  ", myParam1.rgb_scurve_enhance.R_m, myParam1.rgb_scurve_enhance.R_E);
        LCD_ShowString(100, 300, myInform);
        sprintf(myInform, "S������ǿ G: m = %03.2f  E = %02.2f .  ", myParam1.rgb_scurve_enhance.G_m, myParam1.rgb_scurve_enhance.G_E);
        LCD_ShowString(100, 320, myInform);
        sprintf(myInform, "S������ǿ B: m = %03.2f  E = %02.2f .  ", myParam1.rgb_scurve_enhance.B_m, myParam1.rgb_scurve_enhance.B_E);
        LCD_ShowString(100, 340, myInform);
        //��ͣ
        PAUSE();

        //��������
        //myParam1.gray_scurve_enhance._m += 10.0f;
        myParam1.rgb_scurve_enhance.R_E += 0.5f;
        myParam1.rgb_scurve_enhance.G_E += 0.4f;
        myParam1.rgb_scurve_enhance.B_E += 0.3f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





