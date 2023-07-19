#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
#include "../YMCV/CORE/YMCV_PubData.h"
/*******************************************************************************************
    *
    * ˵��   ���������Զ���Ⱦɫ������ǿ����ɫ����ȾΪ����չʾ��ʹ���Զ��庯��������Ⱦ����Ρ��Ҷ�ͼת����ͼ��ʾ���Ҷ�ͼ�Զ�����ɫ��Ⱦ
    *
    ***************************************************************************************/
int32 myRFunction(int32 color);
int32 myGFunction(int32 color);
int32 myBFunction(int32 color);
float32 myFun_Rk = 1.0f, myFun_Gk = 1.0f, myFun_Bk = 1.0f;
int main()
{
    char myInform[100];
    LCD_Init(1000, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//��ɫͼ


    /*--------------------------��ɫͼ�Զ�����Ⱦ��--------------------------------------------------*/


    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Channel_CustomTransform, PIXChannelNum3);//�Զ�����Ⱦ��

    for (int i = 0; i < 10; i++)
    {
        //ʹ���Լ��ĺ�����ʼ����Ⱦͨ��
        YMCV_ThreeChannel_CustomMapTo(myRender, myRFunction, myGFunction, myBFunction);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);
        //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 30, 320, 40);
        //��Ϣ��ʾ
        sprintf(myInform, "�Զ���������ǿ: R��%03.2f G��%03.2f B��%03.2f . ", myFun_Rk, myFun_Gk, myFun_Bk);
        LCD_ShowString(100, 300, myInform);

        //��ͣ
        PAUSE();

        //��������
        myFun_Rk += 1.0f;
        myFun_Gk += 0.8f;
        myFun_Bk += 0.5f;
    }
    //�ͷ���Ⱦ��1
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------��ɫͼ��ɫ��--------------------------------------------------*/

    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);//�Ҷ�ͼ
    CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_ThreeToSingle, PIXChannelNum3);//��ͨ��ת��ͨ��
    //������ɫ��
    color24_t myPalette[256] = { 0 };

    //ʹ��(0.3,0.6,0.1)ϵ��ת��ͨ��
    float32 kr = 0.3, kg = 0.6, kb = 0.1;
    for (int i = 0; i < 256; i++)
    {
        myPalette[i].R = (_color8_t)(kr * i) ;
        myPalette[i].G = (_color8_t)(kg * i) ;
        myPalette[i].B = (_color8_t)(kb * i) ;
    }

    //ʹ�õ�ɫ�������Ⱦ��ͨ�����á���ʼ��
    YMCV_ThreeChannel_PaletteMapTo(myRender2, myPalette);
    //ʹ��ͨ����Ⱦ��������ȾͼƬ
    YMCV_Image_ThreeToSingle_ChannelRenderTo(myImage0, myImage2, myRender2);
    //��ʾͼƬ1
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "�Զ���ϵ��:k=��%01.2f,%01.2f,%01.2f��",kr, kg, kb);
    LCD_ShowString(300, 300, myInform);
    //��ͣ
    PAUSE();

    //�ͷ���Ⱦ��2
    YMCV_Free_PIX_RenderFree(myRender2);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

//�Զ������Ժ���
int32 myRFunction(int32 color)
{
    return myFun_Rk * color;
}
int32 myGFunction(int32 color)
{
    return myFun_Gk * color;
}
int32 myBFunction(int32 color)
{
    return myFun_Bk * color;
}

