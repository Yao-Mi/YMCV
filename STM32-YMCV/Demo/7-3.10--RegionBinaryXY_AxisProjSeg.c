#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� xy��ͶӰ�ָ�Ϊ����չʾ�˶�ֵͼ�ָ��Ч��
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
    uint8* myData = CV_malloc1(sizeof(mnumber0_9_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mnumber0_9_gray, sizeof(mnumber0_9_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 100, CVGrayType);//�Ҷ�ͼ0

	  int sty = myImage0_gray->height;
	  //��ʾͼƬ0
	  LCD_Clear(GRAY);
		LCD_show_image(stx + 0, 0, myImage0_gray);
	  sprintf(myInform, "Gray image");//��Ϣ��ʾ
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);
	
    /*-------------------------- ��ֵ�� --------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//��ֵͼ
    CV_binary_param myParam = { .gray_double_t.thresh_min = 0 ,.gray_double_t.thresh_max = 50 };
    
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);//������ֵ�任��Ⱦ��
    YMCV_BinaryKernel_Load(myRender1, &myParam);//���ز���������Ⱦ���á���ʼ��
    YMCV_Image_ThreshTo_Binary(myImage0_gray, myImage2, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ

    //��ʾ��ֵ�����
    LCD_show_image(stx + 0, 0, myImage2);
    sprintf(myInform, "To Binary");//��Ϣ��ʾ
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);
    
    /*-------------------------- y,xͶӰ�ַ��ָ� --------------------------------------------------*/
    CVVECTORS_U16 yhist = YMCV_Binary_XY_Axis_Proj(myImage2, 1);//�Ȱ�y��ͶӰ
    YMCV_Binary_Proj_Hist_Seg(yhist, 5, 3);//ͶӰֵͳ��С��5������С��3������Ϊ0
    LCDProjHistShow(yhist, 1, 0, 0);//��ʾyͶӰͼ
    //��y�������
    for (int y = 0; y < yhist->dataNum; y++)
    {
        //ȡ��0�����λ��
        if (yhist->datap[y] > 0)
        {
            int ry = y;
            while ((ry < yhist->dataNum)&&(yhist->datap[ry]))//ֱ��������һ��0���ɻ�÷�0Ƭ��
            {
                ry++; 
            }
            //��ȡ����ͼƬ
            CVIMAGE ySegImg = YMCV_Creat_ImgCopyTo(myImage2, 0, y, myImage2->width, ry - y);
            LCD_show_image(stx + 0, sty, ySegImg);

            sprintf(myInform, "Y Seg:");
            LCD_ShowString(0, sty, myInform);
            Delay(500);
						
            //x��ͶӰ����
            CVVECTORS_U16 xhist = YMCV_Binary_XY_Axis_Proj(ySegImg, 0);//ͶӰ��x��
            YMCV_Binary_Proj_Hist_Seg(xhist, 3, 5);//��ͶӰֱ��ͼ���зָ�
            LCDProjHistShow(xhist, 0, stx + 0, sty + ySegImg->height);//��ʾͶӰͼ

            sprintf(myInform, "X Seg:");
            LCD_ShowString(0, sty + ySegImg->height, myInform);
            Delay(500);
            //��x�������
            for (int x = 0; x < xhist->dataNum; x++)
            {
                //ȡ��0�����λ��
                if (xhist->datap[x] > 0)
                {
                    int rx = x;
                    while ((rx < xhist->dataNum) && (xhist->datap[rx]))
                    {
                        rx++;
                    }
                    //��ȡ����ͼƬ
                    CVIMAGE xSegImg = YMCV_Creat_ImgCopyTo(ySegImg, x, 0, rx - x, ySegImg->height);
                    LCD_show_image(stx + x, sty + ySegImg->height*2, xSegImg);
                    Delay(300);
                    //�ͷ��ڴ�
                    YMCV_Free_ImgFree(xSegImg);
                    //�����Ѿ�ȡ��������
                    x = rx;
                }
            }
            
						//�����Ļ����
						LCD_Fill_ConstRgbRect(stx + 0, sty,ySegImg->width,ySegImg->height,(color24_t){255,255,255});
						LCD_Fill_ConstRgbRect(stx + 0, sty + ySegImg->height-1,ySegImg->width,ySegImg->height*2+1,(color24_t){128,128,128});
						//�ͷŽ�ͼ�ڴ��ͶӰͼ
            YMCV_Free_ImgFree(ySegImg);
            YMCV_Free_Vector_U16_Free(xhist);
            //�����Ѿ�ȡ��������
            y = ry;
        }
    }
    //�ͷ��ڴ�
    YMCV_Free_Vector_U16_Free(yhist);
		YMCV_Free_Binary_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);

}

