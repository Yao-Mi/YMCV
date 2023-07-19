
#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FormCge.h"
#include"../YMCV/CORE/YMCV_FreqCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ͼ��dctΪ����չʾ�˽�һ��ͼ�����Ƶ��任������任�Ľ����
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);

    //�����ռ���Ⱦ��
    uint8 kernel_r = 0;
    CVDCT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//�������ֵ

    /*--------------------------�Ҷ�ͼ��dct�任--------------------------------------------------*/

    //����������ͼ���ר�� dct�˲���
    myKernel = YMCV_Creat_DCT2D_Render_Creat(myImage0->width, myImage0->height);
    //ʹ���˲�������dct���ͼ�����ʹ���˲��������Ĳ���������,���ͱ���float����
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myKernel->width, myKernel->height, CVFGrayType);
    //����DCT�任
    YMCV_Gray_Image_DCT(myKernel, myImage0, myImage1);//�������myImage1

    /*--------------------------Ƶ��ͼ����ʾ--------------------------------------------------*/
   //��ʾƵ�׷���ͼƬ2
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_FGrayToGray);

    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "dct �任����ͼ"); //��Ϣ��ʾ
    LCD_ShowString(100, 510, myInform);
    PAUSE();
    /*--------------------------Ƶ��ͼ��idct�任--------------------------------------------------*/

    //����idct����Ľ��ͼ����ԭʼͼһ�¼���
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    //����iFFT�任
    YMCV_Gray_Image_IDCT(myKernel, myImage3, myImage1);//������myImage3
    //��ʾͼƬ2
    LCD_show_image(500, 0, myImage3);
    sprintf(myInform, "idct �任���ͼ"); //��Ϣ��ʾ
    LCD_ShowString(100, 510, myInform);


    //�ͷſռ���Ⱦ��
    YMCV_Free_DCT2D_RenderFree(myKernel);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);

    PAUSE();
    LCD_Destory();
    return 0;
}