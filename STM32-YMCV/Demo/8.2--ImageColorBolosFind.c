
/*******************************************************************************************
    *
    * ˵��  �������� ��ɫ��ͬʱ����Ϊ����չʾ��bolb��ȡ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage2, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ

    /*------------------------ �Ҷ�ɫ����ȡ ------------------------------*/
    CVBLOB_SEG_RENDER blob_seg_render = YMCV_Creat_BlobSegRender_Creat(CV_DoubleThreshold);//������ֵ���ָ���Ⱦ��
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage2);
	
    CV_binary_param myParam[8];
    //���ò���
    myParam[0].gray_double_t.thresh_min = 0; //��һ��
    myParam[0].gray_double_t.thresh_max = 20;
    myParam[1].gray_double_t.thresh_min = 25;//�ڶ���
    myParam[1].gray_double_t.thresh_max = 55;
    myParam[2].gray_double_t.thresh_min = 60;//������
    myParam[2].gray_double_t.thresh_max = 100;
    myParam[3].gray_double_t.thresh_min = 110;//������
    myParam[3].gray_double_t.thresh_max = 150;
    myParam[4].gray_double_t.thresh_min = 160;//������
    myParam[4].gray_double_t.thresh_max = 190;
    myParam[5].gray_double_t.thresh_min = 210;//������
    myParam[5].gray_double_t.thresh_max = 220;
    myParam[6].gray_double_t.thresh_min = 225;//������
    myParam[6].gray_double_t.thresh_max = 238;
    myParam[7].gray_double_t.thresh_min = 240;//�ڰ���
    myParam[7].gray_double_t.thresh_max = 255;

    //����Ⱦ������8�����
    YMCV_BlobSegKernel_Load(blob_seg_render, myParam,8);

    //ɫ�����
    uint8 minArea = 50;
    uint8 showSegX = 2;//��ʾ�ڶ������ݵķָ�ͼ
    CVVECTORS_U16 blobs = YMCV_Image_Find_ColorBlobs(myImage2, blob_seg_render, minArea, showSegX);

    LCD_show_image(stx + 0, myImage2->height, myImage2);
		
		int sty = myImage2->height;
    //��ʾblob��box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        uint8 label = blobp->label;//���������ɫ��ǩ
        //ѡ����Ʋ���box
        if((label+1 == showSegX) || (showSegX == 0))
        LCD_Draw_Rect(stx + blobp->box.x, sty + blobp->box.y, blobp->box.w, blobp->box.h, YMCV_HeatPalette[label*36]);
    }

    //�ͷ�blob�ڴ� ����Ⱦ���ڴ�
    YMCV_Free_Vector_U16_Free(blobs);
    YMCV_Free_BlobSegRender_Free(blob_seg_render);
    //��Ϣ��ʾ
    sprintf(myInform, "Gray Color Box ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);

    /*------------------------ RGB��ɫɫ����ȡ ------------------------------*/
    blob_seg_render = YMCV_Creat_BlobSegRender_Creat(CV_RGB2Threshold);//���´�����ֵ���ָ���Ⱦ��
    CVIMAGE myImage3 = YMCV_Creat_Img_BackUp(myImage0);
		//��ʾ
    LCD_show_image(stx + 0, 0, myImage0);
		sty = myImage0->height;
		
    myParam[0].rgb_double_t.thresh_Rmin = 150; //��һ��
    myParam[0].rgb_double_t.thresh_Rmax = 255;
    myParam[0].rgb_double_t.thresh_Gmin = 30;
    myParam[0].rgb_double_t.thresh_Gmax = 90;
    myParam[0].rgb_double_t.thresh_Bmin = 0;
    myParam[0].rgb_double_t.thresh_Bmax = 80;
    myParam[1].rgb_double_t.thresh_Rmin = 170; //�ڶ���
    myParam[1].rgb_double_t.thresh_Rmax = 230;
    myParam[1].rgb_double_t.thresh_Gmin = 110;
    myParam[1].rgb_double_t.thresh_Gmax = 150;
    myParam[1].rgb_double_t.thresh_Bmin = 80;
    myParam[1].rgb_double_t.thresh_Bmax = 110;
    //����Ⱦ������2�����
    YMCV_BlobSegKernel_Load(blob_seg_render, myParam, 2);

    //ɫ�����
    minArea = 50;
    showSegX = 2;//��ʾ�ڶ������ݵķָ�ͼ
    blobs = YMCV_Image_Find_ColorBlobs(myImage0, blob_seg_render, minArea, showSegX);
    LCD_show_image(stx + 0, sty + 0, myImage0);
    //��ʾblob��box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        uint8 label = blobp->label;//���������ɫ��ǩ
        //ѡ����Ʋ���box
        if ((label + 1 == showSegX) || (showSegX == 0))
            LCD_Draw_Rect(stx + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, YMCV_HeatPalette[label * 36]);
    }

    //�ͷ�blob�ڴ�
    YMCV_Free_Vector_U16_Free(blobs);
    YMCV_Free_BlobSegRender_Free(blob_seg_render);
		
    //��Ϣ��ʾ
    sprintf(myInform, "RGB Color Box  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);
    /*------------------------ LAB��ɫɫ����ȡ ------------------------------*/
    blob_seg_render = YMCV_Creat_BlobSegRender_Creat(CV_LAB2Threshold);//���´�����ֵ���ָ���Ⱦ��
		
		LCD_show_image(stx + 0, 0, myImage3);
		
    myParam[0].lab_double_t.thresh_Lmin = 75; //��һ��
    myParam[0].lab_double_t.thresh_Lmax = 90;
    myParam[0].lab_double_t.thresh_Amin = 0;
    myParam[0].lab_double_t.thresh_Amax = 6;
    myParam[0].lab_double_t.thresh_Bmin = 12;
    myParam[0].lab_double_t.thresh_Bmax = 20;
    myParam[1].lab_double_t.thresh_Lmin = 60; //�ڶ���
    myParam[1].lab_double_t.thresh_Lmax = 80;
    myParam[1].lab_double_t.thresh_Amin = 10;
    myParam[1].lab_double_t.thresh_Amax = 30;
    myParam[1].lab_double_t.thresh_Bmin = 10;
    myParam[1].lab_double_t.thresh_Bmax = 50;
    //����Ⱦ������2�����
    YMCV_BlobSegKernel_Load(blob_seg_render, myParam, 2);

    //ɫ�����
    minArea = 100;
    showSegX = 0;//����ʾ���ݵķָ�ͼ
    blobs = YMCV_Image_Find_ColorBlobs(myImage3, blob_seg_render, minArea, showSegX);
    LCD_show_image(stx + 0, sty + 0, myImage3);
    //��ʾblob��box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        uint8 label = blobp->label;//���������ɫ��ǩ
        //ѡ����Ʋ���box
        if ((label + 1 == showSegX) || (showSegX == 0))
            LCD_Draw_Rect(stx + blobp->box.x, sty + blobp->box.y, blobp->box.w, blobp->box.h, YMCV_HeatPalette[label * 36]);
    }
    //��Ϣ��ʾ
    sprintf(myInform, "LAB Color Box  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);
		
    //�ͷ�blob�ڴ�
    YMCV_Free_Vector_U16_Free(blobs);
    YMCV_Free_BlobSegRender_Free(blob_seg_render);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage3);
}

