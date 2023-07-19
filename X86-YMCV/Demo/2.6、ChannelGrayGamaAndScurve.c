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
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//�Ҷ�ͼ

    /*--------------------------٤��������ǿ--------------------------------------------------*/

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);


    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Gama_Enhance, PIXChannelNum1);//gama������ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     c��5��    ��  gama��0.1��
    CV_pixchannel_1param myParam = { .gray_gama_enhance._c = 309.0f,.gray_gama_enhance._gama = 1.6f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);
        //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 540, 60, 40);
        //��Ϣ��ʾ
        sprintf(myInform, "gama��ǿ: c=%03.2f gama= %01.3f  .", myParam.gray_gama_enhance._c, myParam.gray_gama_enhance._gama);
        LCD_ShowString(100, 300, myInform);

        //��ͣ
        PAUSE();

        //��������
        myParam.gray_gama_enhance._c += 20.0f;
        //myParam.gray_gama_enhance._gama += 0.1f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------S������ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Scurve_Enhance, PIXChannelNum1);//S������ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��10��    ��  b��0.001��    ��   c��0.01��
    CV_pixchannel_1param myParam1 = { .gray_scurve_enhance._m = 132.0f,.gray_scurve_enhance._E = -5.0f }; 
    for (int i = 0; i < 20; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 540, 60, 40);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "S������ǿ: m = %03.2f  E = %02.2f . ", myParam1.gray_scurve_enhance._m, myParam1.gray_scurve_enhance._E);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        //myParam1.gray_scurve_enhance._m += 10.0f;
        myParam1.gray_scurve_enhance._E += 0.5f;
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





