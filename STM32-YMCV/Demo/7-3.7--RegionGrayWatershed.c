
/*******************************************************************************************
    *
    * ˵��  �������� �Զ�TriMap���ֶ�TriMapΪ����չʾ�˷�ˮ��ָ��Ч��
    *
    ***************************************************************************************/
CVIMAGE YMCV_GetTriMap_Demo(CVIMAGE image0_gray);

void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_gray);
	
    /*-------------------------- �Զ�����TriMap --------------------------------------------------*/
    //ʹ��7-3.6���̵ķ�������TriMap
    CVIMAGE TriMapMarkers = YMCV_GetTriMap_Demo(myImage0_gray);

    //��ʾͼƬtrimap
		LCD_show_image(stx + 0, myImage0_gray->height, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "TriMap image creat by (7-3.6)");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ800ms
		Delay(800);

    /*-------------------------- ��ˮ��ָ� --------------------------------------------------*/
    YMCV_Gray_Watershed(myImage0_gray, TriMapMarkers);//��Trimapͼ��ָ�����
		
    //��ʾͼƬ
		LCD_show_image(stx + 0, myImage0_gray->height, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "Watershed Segment Mask Image ");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ800ms
		Delay(800);

    CVIMAGE SegImage = YMCV_Image_Mask(TriMapMarkers, myImage0_gray);//�ָ�ͼ��������ʹ��

    //��ʾͼƬ
		LCD_show_image(stx + 0, myImage0_gray->height, SegImage);
    //��Ϣ��ʾ
    sprintf(myInform, "Segment Mask Image Result   ");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ800ms
		Delay(800);
		
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);

    /*-------------------------- ʹ�ü��̺���� �ֶ�����TriMap --------------------------------------------------*/
    
		//���ڵ�Ƭ��δ�Ӽ��̺���꣬�����ⲿ�ֱ�ɾ���ˣ�ֱ�Ӳ��ù̶��㣬����ϸ��ʹ�òο�PC�ϵ�Demo
		//����������������豸����TriMap���ƣ���ο�PC�ϵĸ�����Demo�Ļ��Ʒ���
		CVpoint frontPoints[100] = {{108,50},{108,48},{108,46},{109,44},{110,41},{110,38},{110,35},{110,33},{110,29},{108,28},{105,29},{104,31},{105,33},{106,38},{105,41},{105,44},{102,47},{102,51},{99,51},{99,44},{99,39},{101,35},{100,31},{97,19},{103,19},{109,20},{115,25},{115,32},{114,43},{114,48},{107,53},{103,55},{102,55},{100,55},{99,55},{94,53},{91,43},{91,41},{91,35},{91,31},{91,29},{91,23},{93,21},{93,15},{90,19},{87,26},{87,33},{87,38},{88,44},{88,49},{88,54},{85,55},{83,41},{82,33},{83,22},{113,29},{76,27},{73,33},{75,40},{77,51},{77,19},{71,22},{71,36},{71,41},{71,51},{75,55},{68,25},{67,31},{66,38},{66,43},{66,49},{66,52},{62,31},{62,41},{62,49},{77,60},{87,60},{95,60},{80,64},{85,65},{93,64},{70,63},{62,71},{55,77},{66,79},{77,79},{91,79},{101,79},{113,79},{123,79},{113,68},{105,61},{98,65},{85,69},{75,69},{89,75},{97,73},{106,72},{103,66},{91,69},};
		CVpoint behindPoints[100] = {{5,7},{6,12},{6,17},{6,19},{6,24},{6,32},{6,37},{6,43},{8,49},{7,54},{11,59},{13,55},{15,47},{16,44},{16,36},{14,30},{13,21},{11,13},{18,7},{7,5},{14,7},{31,5},{23,5},{35,5},{42,5},{50,5},{59,5},{63,5},{74,5},{70,4},{79,4},{83,10},{85,5},{88,5},{98,6},{105,6},{113,5},{118,5},{128,5},{140,5},{147,5},{157,3},{153,10},{148,10},{147,19},{149,16},{149,23},{148,27},{148,32},{148,40},{151,46},{145,47},{151,33},{141,23},{143,31},{143,38},{139,11},{143,11},{142,57},{147,52},{145,64},{147,65},{147,72},{145,77},{145,87},{140,85},{140,77},{140,70},{7,59},{7,71},{6,80},{7,89},{7,99},{7,107},{11,105},{11,96},{11,89},{12,71},{14,61},{21,25},{21,12},{30,16},{141,95},{141,102},{145,99},{133,14},{126,9},{39,11},{50,11},{62,9},{23,29},{32,29},{31,19},{27,37},{21,49},{23,41},{138,35},{137,25},{130,18},{123,13},};
		
		//����trimap����ʼ��
		TriMapMarkers = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers);//��ʼ���е�Ϊ�����״̬
		
		//����ǰ���ͱ�����
    int qval= CVTriMapMaxVl,bval= CVTriMapMinVl;
		for(int i=0;i<100;i++)
		{
			//ǰ����
			int x = frontPoints[i].x - 10;//�ѵ��Ƶ���������̫������
			int y = frontPoints[i].y + 20;
			TriMapMarkers->data[ y * TriMapMarkers->width + x] = qval;
			
			//������
			x = behindPoints[i].x;
			y = behindPoints[i].y;
			TriMapMarkers->data[ y * TriMapMarkers->width + x] = bval;
			
			//�����޸�Ϊ��ͬ��ǰ���򱳾���ǣ���CVTriMapMidle���ָ���������
			if(i%15 == 10)
			{
				 //�޸ı������ֵ�����Ϊ��ͬ����,����+1�Ϳ��ԣ�����Ϊ����ʾЧ������+20
				 bval += 20;
				 if (bval >= CVTriMapMidle)bval = CVTriMapMinVl;
				 //�޸�ǰ�����ֵ��
				 qval -= 20;
         if (qval <= CVTriMapMidle)qval = CVTriMapMaxVl;
			}
		}
		
		//��ʾͼƬtrimap
		LCD_show_image(stx + 0, myImage0_gray->height, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "TriMap image creat by hand  ");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ1000ms
		Delay(1000);
		/*-------------------------- ��ˮ��ָ� --------------------------------------------------*/
    YMCV_Gray_Watershed(myImage0_gray, TriMapMarkers);//��Trimapͼ��ָ�����
    //��ʾͼƬ
		LCD_show_image(stx + 0, myImage0_gray->height, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "Watershed Segment Mask Image ");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ800ms
		Delay(800);
 
    SegImage = YMCV_Image_Mask(TriMapMarkers, myImage0_gray);//�ָ�ͼ��������ʹ��

    //��ʾͼƬ
		LCD_show_image(stx + 0, myImage0_gray->height, SegImage);
    //��Ϣ��ʾ
    sprintf(myInform, "Segment Mask Image Result   ");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ800ms
		Delay(800);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);
		//�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
}



//Trimap����
CVIMAGE YMCV_GetTriMap_Demo(CVIMAGE image0_gray)
{
    /*--------------------------����任����ǰ��ͼ--------------------------------------------------*/
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(image0_gray->width, image0_gray->height, CVBinaryType);

    //������ֵ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��

    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 130,.gray_2tobin.rightThresh = 255 }; //������Ⱦ������
    YMCV_SingleChannel_MapTo(myRender, &myParam);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��  

    //ͼ���ֵ��
    YMCV_Image_ChannelRenderTo(image0_gray, myImage1, myRender);//ʹ��ͨ����Ⱦ��������ȾͼƬ


    //�Զ�ֵͼ���о���任
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);

    //�Ծ���ͼ��ֵ��
    myParam.gray_2tobin.leftThresh = 20;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage2, myImage2, myRender);//��ֵ����Ⱦ

    /*--------------------------����任��������ͼ--------------------------------------------------*/

    //��������ȡ��ǰ���ֱ���ȡ��
    YMCV_Image_Binary_Logic_Cal(myImage1, myImage1, CV_Not);//ͼ1 �߼���  ������������ͼ1

    //�Զ�ֵͼ���о���任
    CVIMAGE myImage3 = YMCV_Binary_Distance_Transform(myImage1);

    //�Ծ���ͼ��ֵ��
    myParam.gray_2tobin.leftThresh = 40;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage3, myImage3, myRender);//��ֵ����Ⱦ

    /*-------------------------- TriMapͼ����--------------------------------------------------*/
    //����ʹ��Chanel��Ⱦ������ı�ͼ�����ͣ����ｫ�Ҷȸ�ʽǿ��תΪ��ֵͼ
    myImage2->type = CVBinaryType;
    myImage3->type = CVBinaryType;
    //ʹ��ǰ��ͼ�ͱ���ͼ������trimapͼ
    CVIMAGE TriMapImage = YMCV_BinaryTo_TriMap(myImage2, myImage3);

    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    return TriMapImage;
}




