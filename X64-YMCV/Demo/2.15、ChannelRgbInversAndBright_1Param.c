#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * ˵��  �������Է�ɫ��������ǿΪ����չʾ��ʹ�� ��ͨ����Ⱦ�� ��Ⱦ��ɫͼ��
    *
    ***************************************************************************************/
int main()
{
    char myInform[50];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//��ɫͼ


    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);


    /*--------------------------��ɫͼ������ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Bright, PIXChannelNum1);//ͼ��������ǿ
    //������Ⱦ������
    CV_pixchannel_1param myParam = { .gray_bright._k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam);

        //ʹ��ͨ����Ⱦ��������Ⱦ��ɫͼƬ
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "������ǿ: %01.2f .", myParam.gray_bright._k);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam.gray_bright._k += 0.5f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------��ɫͼ��ɫ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Invers, PIXChannelNum1);//�Ҷȷ�ɫ
    //������Ⱦ������ ȡֵ��[0.0 -- 1.0]���� ���� [0 --- 255]����
    CV_pixchannel_1param myParam1 = { .gray_invers._k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������Ⱦ��ɫͼƬ
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "��ɫ��ǿ: %01.2f .", myParam1.gray_invers._k);
        LCD_ShowString(100, 300, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam1.gray_invers._k += 0.50f;//[0.0 -- 1.0]
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





