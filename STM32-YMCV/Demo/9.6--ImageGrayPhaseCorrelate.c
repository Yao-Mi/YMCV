#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ��λ���ƥ��Ϊ����չʾͼ��ƽ�ƺ��ƥ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

    /*---------------------------- ͼ��ƽ�� --------------------------------------------*/
    //����ͼƬ0����Ⱦ����
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
                              .perspect.wsize_type = CV_WinSize_Const,//�̶�����
                              .perspect.scale = 1.0f };
    //�޸���Ⱦ����
    myParam.perspect.x_angle = 0;
    myParam.perspect.y_angle = 0;
    myParam.perspect.z_angle = 0;
    //����ƫ�����豣����[���� / 2������ / 2]��Χ�ڣ�
    myParam.perspect.x_move = 90; //ƽ�Ʊ任
    myParam.perspect.y_move = 50;
    myParam.perspect.scale = 1;
    //�������α任��Ⱦ��
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

    //ʹ�ò������м�����Ⱦ���á���ʼ��
    YMCV_GeomKernel_Load(myRender1, &myParam);

    //��Ŀ��ͼ��Ľ�����Ⱦ
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);

		LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 0, myImage0_gray);
		int sty = myImage0_gray->height;
    LCD_show_image(stx + 0, sty + 0, myImage1);
    //��Ϣ��ʾ
    sprintf(myInform, "image move ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);

    /*-------------------------- ��λ���ƥ�� --------------------------------------------------*/
    float32 moveX, moveY;
    YMCV_Image_Gray_PhaseCorrelate(myImage0_gray, myImage1, &moveX, &moveY);//����ƫ��
//    //��ʾͼƬ
//    LCD_show_image(0, 0, myImage0_gray);

    //��Ϣ��ʾ
    sprintf(myInform, "found move:x%.2f y%.2f ", moveX, moveY);
    LCD_ShowString(0, sty, myInform);
    Delay(500);

    //�ͷ��ڴ�
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);

}


