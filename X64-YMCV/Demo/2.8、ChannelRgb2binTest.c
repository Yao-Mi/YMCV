#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * ˵��  �������Ե���˫��ֵΪ����չʾ�� ��ͨ����Ⱦ����������Ⱦ�������������������Ⱦ����ʼ���Լ���ȾͼƬ����ʾ
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//��ɫͼ
    //��ʾͼƬ0
    LCD_show_image(0, 0, myImage0);

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);

    /*--------------------------��ɫͼ����ֵ��ֵ��--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_ThreshToBin, PIXChannelNum3);//��ͨ����ֵ����һ��ͨ��
    //������Ⱦ������
    CV_pixchannel_3param myParam = { .rgb_tobin.R_Thresh = 40 , .rgb_tobin.G_Thresh = 40 , .rgb_tobin.B_Thresh = 40 };
    for (int i = 0; i < 22; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "��ͨ������ֵ:R %03d,G %03d,B %03d .", myParam.rgb_tobin.R_Thresh, myParam.rgb_tobin.G_Thresh, myParam.rgb_tobin.B_Thresh);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        if (i>10)
        {
            if (i > 17)
            {
                myParam.rgb_tobin.G_Thresh += 15;
            }
            else
            {
               myParam.rgb_tobin.R_Thresh += 10;
            }
        }
        else
        {
            myParam.rgb_tobin.B_Thresh += 10;
        }
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------��ɫͼ˫��ֵ��ֵ��--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum3);//��ͨ����ֵ����һ��ͨ��
    //������Ⱦ������
    CV_pixchannel_3param myParam1 = {   .rgb_2tobin.R_leftThresh = 0 ,.rgb_2tobin.R_rightThresh = 40,
                                        .rgb_2tobin.G_leftThresh = 0 ,.rgb_2tobin.G_rightThresh = 40 ,
                                        .rgb_2tobin.B_leftThresh = 0 ,.rgb_2tobin.B_rightThresh = 40 };
    for (int i = 0; i < 22; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "��ͨ��˫��ֵ: R %03d--%03d  G %03d--%03d  B %03d--%03d .", myParam1.rgb_2tobin.R_leftThresh, myParam1.rgb_2tobin.R_rightThresh, 
                                                                                    myParam1.rgb_2tobin.G_leftThresh, myParam1.rgb_2tobin.G_rightThresh, 
                                                                                    myParam1.rgb_2tobin.B_leftThresh, myParam1.rgb_2tobin.B_rightThresh );
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        if (i > 10)
        {
            if (i > 17)
            {
                myParam1.rgb_2tobin.G_leftThresh += 25;
                myParam1.rgb_2tobin.G_rightThresh += 25;
            }
            else
            {
                myParam1.rgb_2tobin.R_leftThresh += 10;
                myParam1.rgb_2tobin.R_rightThresh += 10;
            }
        }
        else
        {
            myParam1.rgb_2tobin.B_leftThresh += 10;
            myParam1.rgb_2tobin.B_rightThresh += 10;
        }
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





