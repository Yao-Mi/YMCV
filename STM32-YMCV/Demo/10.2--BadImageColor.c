#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ͼ��dctΪ����չʾ�˽�һ��ͼ���ɳ¾�ͼ���Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ

    myData = CV_malloc1(sizeof(mm02));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02, sizeof(mm02));
    CVIMAGE myImage4 = YMCV_Creat_DataToImg((uint8*)myData,160, 120, CVRgb16Type);//�Ҷ�ͼ
    //��ʾԭͼ
	  LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;
    //����DCT��Ⱦ��
    uint8 kernel_r = 0;
    CVDCT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//�������ֵ

    /*--------------------------�Ҷ�ͼ��dct�任--------------------------------------------------*/

    //����������ͼ���ר�� dct�˲���
    myKernel = YMCV_Creat_DCT2D_Render_Creat(myImage0_gray->width, myImage0_gray->height);
    //ʹ���˲�������dct���ͼ�����ʹ���˲��������Ĳ���������,���ͱ���float����
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myKernel->width, myKernel->height, CVFGrayType);
    //����DCT�任
    YMCV_Gray_Image_DCT(myKernel, myImage0_gray, myImage1);//�������myImage1

    /*--------------------------Ƶ��ͼ����ʾ--------------------------------------------------*/
   //��ʾƵ�׷���ͼƬ2
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    float32* dap = (float32*)myImage1->data;

    //Ƶ��ͼ�����
    for (int i = 0; i < myImage1->height; i++)
    {
        for (int j = 0; j < myImage1->width; j++)
        {
            if ((dap[i * myImage1->width + j]) < 10)
            {
                dap[i * myImage1->width + j] = 0;
            }
        }
    }
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_FGrayToGray);

    LCD_show_image(stx + 0, sty, myImage2);
    sprintf(myInform, "dct result"); //��Ϣ��ʾ
    LCD_ShowString(stx + 0, sty, myInform);
    Delay(300);

    /*--------------------------Ƶ��ͼ��idct�任--------------------------------------------------*/

    //����idct����Ľ��ͼ����ԭʼͼһ�¼���
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    //����iFFT�任
    YMCV_Gray_Image_IDCT(myKernel, myImage3, myImage1);//������myImage3
    //��ʾͼƬ4
    LCD_show_image(stx + 0, 0, myImage4);
    Delay(200);
		//���Ҷ�Ⱦ��ԭ��ɫͼ��
    YMCV_Gray_ColorTo_Rgb16(myImage3, myImage4);
		//����Ч��
    LCD_show_image(stx + 0, sty, myImage4);
    sprintf(myInform, "polluting effects"); //��Ϣ��ʾ
    LCD_ShowString(stx + 0, sty, myInform);

    Delay(500);
    //�ͷſռ���Ⱦ��
    YMCV_Free_DCT2D_RenderFree(myKernel);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(myImage4);
}

