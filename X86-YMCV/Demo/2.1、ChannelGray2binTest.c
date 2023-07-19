#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * ˵��  �������Ե���˫��ֵΪ����չʾ�� ͨ����Ⱦ����������Ⱦ�������������������Ⱦ����ʼ���Լ���ȾͼƬ����ʾ
    *
    ***************************************************************************************/
int main()
{
    char myInform[50];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//�Ҷ�ͼ


    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);


    /*--------------------------����ֵ��ֵ��--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_ThreshToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��
    //������Ⱦ������
    CV_pixchannel_1param myParam = { .gray_tobin.Thresh = 20 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender,&myParam);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "��ͨ������ֵ: %03d .", myParam.gray_tobin.Thresh);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam.gray_tobin.Thresh += 10;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------˫��ֵ��ֵ��--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��
    //������Ⱦ������
    CV_pixchannel_1param myParam1 = { .gray_2tobin.leftThresh = 0 ,.gray_2tobin.rightThresh = 20 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "��ͨ��˫��ֵ: %03d --  %03d .", myParam1.gray_2tobin.leftThresh, myParam1.gray_2tobin.rightThresh);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam1.gray_2tobin.leftThresh += 10;
        myParam1.gray_2tobin.rightThresh += 10;
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





