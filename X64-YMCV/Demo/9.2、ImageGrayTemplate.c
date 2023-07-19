#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ģ��ƥ��Ϊ����չʾ���ַ���ƥ��Ľ��
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

    /*--------------------------expָ����ǿ--------------------------------------------------*/
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);

    //���´���һ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//ָ����ǿ
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };

    YMCV_SingleChannel_MapTo(myRender, &myParam1);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
    YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);

    //��ȡ����ͼ��
    CVIMAGE imgFind = YMCV_Creat_ImgCopyTo(myImage1, 60, 40, 35, 30);

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(300, 30, imgFind);

    //��Ϣ��ʾ
    sprintf(myInform, "expָ����ǿȡģ�� ");
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);
    /*-------------------------- MAD ģ��ƥ�����--------------------------------------------------*/

    //ƽ�����Բ 
    CVrect result = YMCV_Gray_Matching_Template(myImage0, imgFind, CV_Template_MAD, 5, 20); //�����صĿ����0����˵��������

    //��ʾ���
    color24_t rgbval = { .R = 255 };
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //��Ϣ��ʾ
    sprintf(myInform, "   ģ�� MAD ƥ��      ");
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();

    /*-------------------------- SSDA ģ��ƥ�����--------------------------------------------------*/

    //ƽ�����Բ 
    result = YMCV_Gray_Matching_Template(myImage0, imgFind, CV_Template_SSDA, 5, 20); //�����صĿ����0����˵��������

    //��ʾ���
    LCD_show_image(0, 0, myImage0);
    rgbval.G = 255;
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //��Ϣ��ʾ
    sprintf(myInform, "   ģ�� SSDA ƥ��      ");
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();

    /*-------------------------- NCC ģ��ƥ�����--------------------------------------------------*/
    //ƽ�����Բ 
    result = YMCV_Gray_Matching_Template(myImage0, imgFind, CV_Template_NCC, 5, 20); //�����صĿ����0����˵��������

    //��ʾ���
    LCD_show_image(0, 0, myImage0);
    rgbval.R = 0;
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //��Ϣ��ʾ
    sprintf(myInform, "   ģ�� NCC ƥ��      ");
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(imgFind);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





