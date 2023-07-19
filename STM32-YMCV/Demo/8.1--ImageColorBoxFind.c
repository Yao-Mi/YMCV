
/*******************************************************************************************
    *
    * ˵��  �������� ɫ�����Ϊ����չʾ��bolb��ȡ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage2, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
    color24_t thiscolor = { .G = 255 };

    /*------------------------ �Ҷ�ɫ����ȡ ------------------------------*/
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage2);

    CVcolor minVal,maxVal;
    minVal.gray = 100;
    maxVal.gray = 150;

    CVVECTORS_U16 blobs= YMCV_Image_ColorBox_Find(myImage2, minVal, maxVal, 50,1);
    LCD_show_image(stx + 0, myImage2->height, myImage2);
		
		int sty = myImage2->height;
		
    //��ʾblob��box
    for (int i = 0; (blobs!=NULL)&&(i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        LCD_Draw_Rect(stx + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
        LCD_Draw_Rect( stx + blobp->box.x,  sty + blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }

    //�ͷ�blob�ڴ�
    YMCV_Free_Vector_U16_Free(blobs);
		//��Ϣ��ʾ
    sprintf(myInform, "Gray Color Box ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(800);

    /*------------------------ ��ɫɫ����ȡ ------------------------------*/
    LCD_show_image(stx + 0, 0, myImage0);
		sty = myImage0->height;
		
    minVal.rgb16 = YMCV_Rgb3ToRgb565(100, 60, 40);
    maxVal.rgb16 = YMCV_Rgb3ToRgb565(200, 150, 100);
    blobs = YMCV_Image_ColorBox_Find(myImage0, minVal, maxVal, 100,1);
		
		LCD_show_image(stx + 0, sty + 0, myImage0);
    //��ʾblob��box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        LCD_Draw_Rect(stx + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
        LCD_Draw_Rect(stx + blobp->box.x, sty + blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }
    //��Ϣ��ʾ
    sprintf(myInform, "RGB Color Box  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(800);
		
    //�ͷ�blob�ڴ�
    YMCV_Free_Vector_U16_Free(blobs);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage2);
		
}

