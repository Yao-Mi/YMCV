#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * ˵��  �������ԶԱȶȡ��ֶ�������ǿΪ����չʾ�˲���ȡֵ �������������Լ�ͨ��ת��ʾ��ͼ����ʾ
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

    /*--------------------------��ɫͼ�Աȶ���ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Contract, PIXChannelNum3);//ͼ��Աȶ���ǿ/����������
    //������Ⱦ������
    CV_pixchannel_3param myParam = { .rgb_contract.R_k = -100.0f, .rgb_contract.G_k = -100.0f, .rgb_contract.B_k = -100.0f };
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
        sprintf(myInform, "�Աȶ���ǿ: R:%01.2f G:%01.2f B:%01.2f .   ", myParam.rgb_contract.R_k, myParam.rgb_contract.G_k, myParam.rgb_contract.B_k);
        LCD_ShowString(100, 300, myInform);

        //��ͣ
        PAUSE();

        //��������
        myParam.rgb_contract.R_k += 20.0f;
        myParam.rgb_contract.G_k += 15.0f;
        myParam.rgb_contract.B_k += 10.0f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------��ɫͼ�ֶ�������ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Liner_Enhance, PIXChannelNum3);//�ֶ�������ǿ
    //������Ⱦ������ ��[0,f_a][f_a,f_b][f_b,f_c][f_c,255] �任Ϊ [0,t_a][t_a,t_b][t_b,t_c][t_c,255] 
    CV_pixchannel_3param myParam1 = { .rgb_liner_enhance.R_from_a = 20,.rgb_liner_enhance.R_from_b = 100,.rgb_liner_enhance.R_from_c = 200,//R����
                                      .rgb_liner_enhance.R_to_a = 30, .rgb_liner_enhance.R_to_b = 30, .rgb_liner_enhance.R_to_c = 230 ,
                                      .rgb_liner_enhance.G_from_a = 20,.rgb_liner_enhance.G_from_b = 100,.rgb_liner_enhance.G_from_c = 200,//G����
                                      .rgb_liner_enhance.G_to_a = 30, .rgb_liner_enhance.G_to_b = 30, .rgb_liner_enhance.G_to_c = 230 ,
                                      .rgb_liner_enhance.B_from_a = 20,.rgb_liner_enhance.B_from_b = 100,.rgb_liner_enhance.B_from_c = 200,//B����
                                      .rgb_liner_enhance.B_to_a = 30, .rgb_liner_enhance.B_to_b = 30, .rgb_liner_enhance.B_to_c = 230 , };
    for (int i = 0; i < 10; i++)
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
        sprintf(myInform, "�ֶ�������ǿ  R:[ %03d %03d %03d]-> [ %03d %03d %03d]   ",
            myParam1.rgb_liner_enhance.R_from_a, myParam1.rgb_liner_enhance.R_from_b, myParam1.rgb_liner_enhance.R_from_c, \
            myParam1.rgb_liner_enhance.R_to_a, myParam1.rgb_liner_enhance.R_to_b, myParam1.rgb_liner_enhance.R_to_c); \
            LCD_ShowString(100, 300, myInform);
        sprintf(myInform, "�ֶ�������ǿ  G:[ %03d %03d %03d]-> [ %03d %03d %03d]   ",
            myParam1.rgb_liner_enhance.G_from_a, myParam1.rgb_liner_enhance.G_from_b, myParam1.rgb_liner_enhance.G_from_c, \
            myParam1.rgb_liner_enhance.G_to_a, myParam1.rgb_liner_enhance.G_to_b, myParam1.rgb_liner_enhance.G_to_c); \
            LCD_ShowString(100, 320, myInform);
        sprintf(myInform, "�ֶ�������ǿ  B:[ %03d %03d %03d]-> [ %03d %03d %03d]   ",
            myParam1.rgb_liner_enhance.B_from_a, myParam1.rgb_liner_enhance.B_from_b, myParam1.rgb_liner_enhance.B_from_c, \
            myParam1.rgb_liner_enhance.B_to_a, myParam1.rgb_liner_enhance.B_to_b, myParam1.rgb_liner_enhance.B_to_c); \
            LCD_ShowString(100, 340, myInform);
        //��ͣ
        PAUSE();

        //�������� һ��ѡ��ù̶���from�����󣬵���to���������в�����Χ��[0 -- 255]
        myParam1.rgb_liner_enhance.R_to_b += 20;//[to_a -- to_c]
        myParam1.rgb_liner_enhance.G_to_b += 15;
        myParam1.rgb_liner_enhance.B_to_b += 10;
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





