#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ����ƥ��LM�㷨Ϊ����չʾ���Ӳ�ͼ����Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(sleft1_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)sleft1_gray, sizeof(sleft1_gray));
    CVIMAGE myImageL = YMCV_Creat_DataToImg((uint8*)myData, 120, 120, CVGrayType);//�Ҷ�ͼ0

    myData = CV_malloc1(sizeof(sright1_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)sright1_gray, sizeof(sright1_gray));
    CVIMAGE myImageR = YMCV_Creat_DataToImg((uint8*)myData, 120, 120, CVGrayType);//�Ҷ�ͼ0
     
    /*-------------------------- �Ӳ�ͼ����--------------------------------------------------*/

	  LCD_Clear(GRAY);
    LCD_show_image(0, 0, myImageL);//ԭͼ��ʾ�����
    LCD_show_image(myImageL->width, 0, myImageR);//ԭͼ��ʾ���ұ�
	  
	  int sty = myImageL->width;
	
    CVIMAGE dImg = YMCV_Gray_Stereo_LineMatching(myImageL, myImageR, 10, 6);//�����Ӳ����

    CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_SingleToThree, PIXChannelNum3);//��ͨ��ת��ͨ��
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(dImg->width, dImg->height, CVRgb16Type);
    //ʹ������ͼ ��ɫ�������Ⱦ��ͨ�����á���ʼ��
    YMCV_ThreeChannel_PaletteMapTo(myRender2, (color24_t*)&YMCV_HeatPalette);
    //ʹ��ͨ����Ⱦ��������ȾͼƬ
    YMCV_Image_SingleToThree_ChannelRenderTo(dImg, myImage3, myRender2);
   
    LCD_show_image(stx + 0, sty, myImage3);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, "  StereoLM  ");
    LCD_ShowString(stx + 0, sty, myInform);
    Delay(500);
    //�ͷ��ڴ�
    YMCV_Free_PIX_RenderFree(myRender2);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImageL);
    YMCV_Free_ImgFree(myImageR);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(dImg);

}





