#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * ˵��  ���������ع���ֵ���Ҷ�ȾɫΪ����չʾ�� ��Ⱦ����ʹ�ã��Լ��Ҷ�Ⱦɫ�����ֲ���ȡֵ��
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
    //��ʾͼƬ0
    LCD_show_image(0, 0, myImage0);


    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);

    /*--------------------------��ɫͼ��ֵ�ع�--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Expose, PIXChannelNum3);//ͼ���ع�
    //������Ⱦ������
    CV_pixchannel_3param myParam = { .rgb_expose.R_thresh = 0,.rgb_expose.G_thresh = 0,.rgb_expose.B_thresh = 0 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "�ع���ֵ: R %03d  G %03d  B %03d .", myParam.rgb_expose.R_thresh, myParam.rgb_expose.G_thresh, myParam.rgb_expose.B_thresh);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam.rgb_expose.R_thresh += 10;
        myParam.rgb_expose.G_thresh += 10;
        myParam.rgb_expose.B_thresh += 10;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------��ɫͼȾɫ��1--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_ColorTo, PIXChannelNum3);//��ɫ��ͨ��Ⱦɫ
    //������Ⱦ������ ȡֵ��[0.0 -- 1.0]���� ���� [0 --- 255]����
    CV_pixchannel_3param myParam1 = { .rgb_colorto.R_color = 0.0 , .rgb_colorto.G_color = 0.0 , .rgb_colorto.B_color = 0.0 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "ͼƬȾɫ: R��%03.2f%%  G��%03.2f%%  B��%03.2f%% ", myParam1.rgb_colorto.R_color, myParam1.rgb_colorto.G_color, myParam1.rgb_colorto.B_color);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam1.rgb_colorto.R_color +=  0.11f;//[0.0 -- 1.0]
        myParam1.rgb_colorto.G_color +=  0.08f;
        myParam1.rgb_colorto.B_color +=  0.07f;
    }

    /*--------------------------��ɫͼȾɫ��2--------------------------------------------------*/

    //���³�ʼ��Ⱦɫ����
    myParam1.rgb_colorto.R_color = 0.0f;
    myParam1.rgb_colorto.G_color = 0.0f;
    myParam1.rgb_colorto.B_color = 0.0f;

    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "ͼƬȾɫ: R��%03.2f  G��%03.2f  B��%03.2f . ", myParam1.rgb_colorto.R_color, myParam1.rgb_colorto.G_color, myParam1.rgb_colorto.B_color);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam1.rgb_colorto.R_color += 13.0f;//ȡֵ�� [0 -- 255]����
        myParam1.rgb_colorto.G_color += 19.0f;
        myParam1.rgb_colorto.B_color += 26.0f;
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


