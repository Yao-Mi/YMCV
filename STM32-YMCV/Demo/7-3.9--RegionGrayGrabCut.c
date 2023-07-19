#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� GrabCutΪ����չʾ���Զ��ָ�ָ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
//    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
//    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
//		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_g = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0
	
	  //��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_g);
	  int sty = myImage0_g->height;
		//ԭͼ̫���ˣ���Сһ�룬��80x60���д���
		CV_geom_param myParam = {.stretch.Imgin_w= myImage0_g->width,
													 .stretch.Imgin_h = myImage0_g->height, 
													 .stretch.out_w = myImage0_g->width/2, 
													 .stretch.out_h = myImage0_g->height/2, };

		CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Stretch);//�������α任��Ⱦ��
		YMCV_GeomKernel_Load(myRender1, &myParam);//ʹ�ò������м�����Ⱦ���á���ʼ��
		CVIMAGE myImage0_gray = YMCV_Image_Geom_RenderTo(myImage0_g,myRender1);//���м��α任��Ⱦ
    
		//��ʾͼƬ0
		LCD_show_image(0, 0, myImage0_gray);
		//��Ϣ��ʾ
    sprintf(myInform, "half size");
    LCD_ShowString(myImage0_gray->width, 0, myInform);

		//�ͷ���Ⱦ��
		YMCV_Free_GEOM_RenderFree(myRender1);
    /*-------------------------- �ֶ�����TriMap--------------------------------------------------*/
	
    color24_t thiscolor = { .R = 255,.G = 0 };
    LCD_Point_Color_Set(thiscolor);

    CVpoint myPoint[]={{23,4},{23,24},{31,60},{30,88},{31,95},{31,115},{101,115},{107,118},{121,91},{121,65},{120,35},{115,14},{118,0},};
    uint16 num = sizeof(myPoint) / sizeof(CVpoint);
		
		int i;
		//ԭͼ��С��һ����д�������Ҳ��Сһ��
		for(int i=0;i<num;i++)
		{
			myPoint[i].x /=2;
			myPoint[i].y /=2;
		}
			
		//��������
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(myPoint[i].x, myPoint[i].y, myPoint[i + 1].x, myPoint[i + 1].y);
    }
    LCD_Draw_Line(myPoint[i].x, myPoint[i].y, myPoint[0].x, myPoint[0].y);
		

    CVIMAGE TriMapMarkers = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers); //trimap��ʼ��

    CVcolor fillval;
    //grabCut ʹ�õ����ֵ��ǰ����[CVTriMapMinVl,CVTriMapMidle)��������(CVTriMapMidle,CVTriMapMaxVl]
    //���У��ٷְ�Ϊǰ���ģ�ȡCVTriMapMinVl���ٷְٱ�����ȡCVTriMapMaxVl��
    fillval.gray = CVTriMapMaxVl - 10;//����Ϊǰ����ֵ

    //��Trimap�����������
    YMCV_Img_Scanline_AreaFill(TriMapMarkers, myPoint, num, fillval);

    //��ʾͼƬtrimap
		LCD_show_image(0, myImage0_gray->height, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "TriMap creat");
    LCD_ShowString(myImage0_gray->width, myImage0_gray->height, myInform);
//		//��ͣ800ms
//		Delay(800);

    /*-------------------------- grabCut�ָ� --------------------------------------------------*/
    YMCV_Gray_GrabCut(myImage0_gray, TriMapMarkers, 2);//���ε���

    //��ʾͼƬ
		LCD_show_image(0, myImage0_gray->height, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "GrabCut Result");
    LCD_ShowString(myImage0_gray->width, myImage0_gray->height, myInform);
		//��ͣ500ms
		Delay(500);

    /*-------------------------- ���չʾ --------------------------------------------------*/
    //���ָ���ͼx2��ԭ�����±��160x120��С
		CV_geom_param myParam2 = {.stretch.Imgin_w= TriMapMarkers->width,
													 .stretch.Imgin_h = TriMapMarkers->height, 
													 .stretch.out_w = myImage0_g->width, 
													 .stretch.out_h = myImage0_g->height, };

		CVGEOM_RENDER myRender2 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);//�������α任��Ⱦ��
		YMCV_GeomKernel_Load(myRender2, &myParam2);//ʹ�ò������м�����Ⱦ���á���ʼ��
		CVIMAGE myImage0_mask = YMCV_Image_Geom_RenderTo(TriMapMarkers,myRender2);//���м��α任��Ⱦ
    
		//��ʾͼƬ0
		LCD_show_image(stx + 0, sty, myImage0_mask);
		//��ͣ1000ms
		Delay(1000);

    //
    CVIMAGE SegImage = YMCV_Image_Mask(myImage0_mask, myImage0_g);//�ָ�ͼ��������ʹ��

    //��ʾͼƬ
		LCD_show_image(stx + 0, sty, SegImage);
    sprintf(myInform, "GrabCut Result Mask");//��Ϣ��ʾ
    LCD_ShowString(stx + 0, sty, myInform);
		//��ͣ1000ms
		Delay(1000);

		//�ͷ���Ⱦ��
		YMCV_Free_GEOM_RenderFree(myRender2);
		
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage0_mask);
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);

}

