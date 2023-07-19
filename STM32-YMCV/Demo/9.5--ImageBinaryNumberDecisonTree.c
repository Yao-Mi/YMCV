#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ID3 ����������Ϊ����չʾ������ʶ���Ч��
    *
    ***************************************************************************************/
void NumberDecisionTree_Train();

void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
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
    
    /*-------------------------- ������ģ�ͼ��� --------------------------------------------------*/
    const uint8 decision_model[] = "[0,0,0,14]LR[16,0,0,18]LR[29,0,0,12]LR[16,0,0,13]LR[29,0,0,8]LR[11,0,0,20]LR[14,0,0,16]LR[9,1,0,0]--[5,1,0,-1088]--[14,0,0,21]LR[6,1,0,-960]--[7,1,0,-896]--[29,0,1,8]LR[3,1,0,0]--[2,1,0,-896]--[4,1,0,0]--[1,1,0,-1024]--[0,1,0,0]--[8,1,0,-1344]--";
    CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//����ID3����ģ��
    YMCV_DecisionTree_Load(myID3, (uint8*)decision_model);//ģ�ͼ���

    /*-------------------------- y,xͶӰ�ַ��ָ����ȡ�������� --------------------------------------------------*/
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
										/*-------------------------- ����������ȡ--------------------------------------------------*/
                    //��ͼ���Ϊ16��16��С
                    CVIMAGE resultImg = YMCV_Binary_MaxPooling_ScaleSizeTo_MxM(xSegImg, 16);
                    LCD_show_image(120, 200, resultImg);
                    CVVECTORS_U16 numberhist = YMCV_Binary_Number16x16_ProjHist16(resultImg);//1x16
										LCD_Fill_ConstRgbRect(200,180,16,60,(color24_t){128,128,128});//�������
                    LCDProjHistShow(numberhist, 0, 200, 180);//��ʾͶӰ����ͼ
                    sprintf(myInform, "16x1");
                    LCD_ShowString(200, 160, myInform);
                    //ǿ��תΪ16x1
                    numberhist->dim = numberhist->dataNum;
                    numberhist->dataNum = 1;
										
                    uint8 lable;
                    YMCV_DecisionTree_Classify(numberhist, &lable,myID3);//�������б�
                    sprintf(myInform, "result %d ", lable);
                    LCD_ShowString(0, 200, myInform);

                    YMCV_Free_ImgFree(resultImg);
                    YMCV_Free_Vector_U16_Free(numberhist);
                    /*-----------------------------------------------------------------------------------------------*/  
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
    Delay(300);

		//�ͷ�ģ���ڴ�
    YMCV_DecisionTree_Free(myID3);
    //�ͷ��ڴ�
    YMCV_Free_Vector_U16_Free(yhist);
		YMCV_Free_Binary_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
}


//����ʶ�������ѵ�������õݹ�ķ�ʽѵ������ջ����Ҫ��������ݽ϶�Ļ�����ŵ�PC�˽���ѵ��
//����ֱ�ӷ��ڵ�Ƭ����ѵ��
void NumberDecisionTree_Train()
{
	 //�򵥵�10�����ݼ�������Խ��Ч����Խ��
	 const uint8 labels[10] = {1,3,2,4,5,7,6,8,0,9};//
	 const int16 mydata[10][16]={ // 10�����ݣ�ÿ��������16ά�ȵ�����
		{ 5, 7, 5,46,26, 2, 2, 1,14,10, 8, 8, 8, 8, 8,29,},//1
		{13,13, 5, 7,10,16,22,10,17,14, 7,11, 8,10,14,16,},//3
		{12,14,10, 9, 9,11,17,15,17,13,10, 6, 7, 5,10,28,},//2
		{ 5, 8, 9, 8,23,34, 5, 4, 7,11,10,12,12,20, 6,17,},//4
		{14,17, 7, 8, 9,10,17,14,26, 3, 7,17, 8, 8,13,14,},//5
		{ 8, 5,15,24,18,11, 9, 5,37,10, 7, 7, 8, 8,11, 9,},//7
		{18,19, 9, 6, 7,11,18, 7,13,10, 7,19,12,11,12,11,},//6
		{20,13, 6, 7, 7,12,22, 9,16, 8,11,17,15, 8, 8,14,},//8
		{21,16, 6, 3, 4, 8,22,17,14,12,11,10,11,11,14,12,},//0
		{16,14, 8, 4, 7,15,20,12,14,10, 9,14,19, 6,10,13,},//9
	 };
	CVVECTORS_U16 myvec = YMCV_Creat_Vector_U16(16, 10);//����16ά������һ��10��

	for (int i = 0; i < myvec->dataNum; i++)//��ʼ������ֵ
	{
		int16* thisvec = (int16*)(myvec->datap + i * myvec->dim);
		//�������ݵ�������
		for (int j = 0; j < myvec->dim;j++)
		{
			thisvec[j]=mydata[i][j];
		}
	}
	 
	 CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//����ID3����ģ��
	 //ID3����������ѵ��
   YMCV_DecisionTree_ID3_Trains(myID3, myvec, (uint8*)labels);
	 //ģ�Ͳ���
	 {
			uint8 label10[10];
			//���������ݷ���
			YMCV_DecisionTree_Classify(myvec, label10,myID3);
		  for(int i = 0;i < 10; i++)
		   printf("ԭʼ��ǩ��%d �������ı�ǩ��%d\n",labels[i],label10[i]);
	 }
	
	 printf("\n");
	 //����ģ��
	 YMCV_DecisionTree_Save(myID3);
	 printf("\n");
   YMCV_Free_Vector_U16_Free(myvec);
	 
   YMCV_DecisionTree_Free(myID3);//�ͷ�ģ���ڴ�
}
