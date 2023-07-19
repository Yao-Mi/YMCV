#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ����任Ϊ����չʾ�˻���ռ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 80;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tuxin02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)tuxin02_gray, sizeof(tuxin02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

	  LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty =  myImage0_gray->height;
        /*--------------------------�Ҷ�Sobel��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//��ֵͼ
    CV_edge_param myParam = { .EdgeType = CV_Sobel ,.Thresh.sobel.minT = 100 };
    YMCV_Gray_EdgeDetect(myImage0_gray, myImage1, &myParam);

    //��ʾͼƬ
    LCD_show_image(stx + 0, sty + 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "Edge Check ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    //��ͣ
    Delay(300);

    /*-------------------------- hough �ռ����--------------------------------------------------*/
    CVIMAGE houghImg = YMCV_Binary_To_Hough(myImage1, 0);//תΪhough�ռ� �Ƕȷֱ���Ϊ (2^0)��

    //��ʾ����ռ�
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(houghImg->width, houghImg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(houghImg, myImage2, CV_IGrayToGray);

    //��ʾͼƬ2
		{
			CV_geom_param stretchParam = {.stretch.Imgin_w= myImage2->width,
															 .stretch.Imgin_h = myImage2->height, 
															 .stretch.out_w = myImage2->width/2,  //���ŵ�һ���С
															 .stretch.out_h = myImage2->height/2, };
			//�������ź���ʾ
			CVGEOM_RENDER stretchRender = YMCV_Creat_GeomRender_Creat(CV_Stretch);	
			YMCV_GeomKernel_Load(stretchRender, &stretchParam);
			CVIMAGE stretchImage = YMCV_Image_Geom_RenderTo(myImage2, stretchRender);														 
														
			LCD_show_image(0, 0, stretchImage);
		  
			//�ͷ���ʱ�ڴ�
			YMCV_Free_GEOM_RenderFree(stretchRender);
			YMCV_Free_ImgFree(stretchImage);
		}

    
        //��Ϣ��ʾ
    sprintf(myInform, " hough space ");
    LCD_ShowString(0, 0, myInform);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(houghImg);

    Delay(300);
}





