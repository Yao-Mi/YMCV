#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"

#include"../YMCV/CORE/YMCV_SegnmCal.h"

/*******************************************************************************************
    *
    * ˵��  �������� ����任Ϊ����չʾ�˶�ֵͼ�任Ч��
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    /*--------------------------��ֵͼ����任--------------------------------------------------*/

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);

    //������ֵ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��
    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 0,.gray_2tobin.rightThresh=150 };    //������Ⱦ������
    YMCV_SingleChannel_MapTo(myRender, &myParam);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��  

    //ͼ���ֵ��
    YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);//ʹ��ͨ����Ⱦ��������ȾͼƬ


    //�Զ�ֵͼ���о���任
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);


    LCD_Clear(0);
    //��ʾͼƬ0
    LCD_show_image(0, 0, myImage1);
    //��ʾͼƬ2
    LCD_show_image(500, 0, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "��ֵͼ����任  ");
    LCD_ShowString(100, 310, myInform);
    //��ͣ
    PAUSE();

    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}









