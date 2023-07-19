#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 ID3 决策树分类为例，展示了数字识别的效果
    *
    ***************************************************************************************/
void NumberDecisionTree_Train();

void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mnumber0_9_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mnumber0_9_gray, sizeof(mnumber0_9_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 100, CVGrayType);//灰度图0

	  int sty = myImage0_gray->height;
	  //显示图片0
	  LCD_Clear(GRAY);
		LCD_show_image(stx + 0, 0, myImage0_gray);
	  sprintf(myInform, "Gray image");//信息显示
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);

    /*-------------------------- 二值化 --------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//二值图
    CV_binary_param myParam = { .gray_double_t.thresh_min = 0 ,.gray_double_t.thresh_max = 50 };
    
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);//创建二值变换渲染器
    YMCV_BinaryKernel_Load(myRender1, &myParam);//加载参数进行渲染配置、初始化
    YMCV_Image_ThreshTo_Binary(myImage0_gray, myImage2, myRender1);//将目标图像的进行渲染

    //显示二值化结果
    LCD_show_image(stx + 0, 0, myImage2);
    sprintf(myInform, "To Binary");//信息显示
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);
    
    /*-------------------------- 决策树模型加载 --------------------------------------------------*/
    const uint8 decision_model[] = "[0,0,0,14]LR[16,0,0,18]LR[29,0,0,12]LR[16,0,0,13]LR[29,0,0,8]LR[11,0,0,20]LR[14,0,0,16]LR[9,1,0,0]--[5,1,0,-1088]--[14,0,0,21]LR[6,1,0,-960]--[7,1,0,-896]--[29,0,1,8]LR[3,1,0,0]--[2,1,0,-896]--[4,1,0,0]--[1,1,0,-1024]--[0,1,0,0]--[8,1,0,-1344]--";
    CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//创建ID3决策模型
    YMCV_DecisionTree_Load(myID3, (uint8*)decision_model);//模型加载

    /*-------------------------- y,x投影字符分割，并提取数字特征 --------------------------------------------------*/
		CVVECTORS_U16 yhist = YMCV_Binary_XY_Axis_Proj(myImage2, 1);//先按y轴投影
    YMCV_Binary_Proj_Hist_Seg(yhist, 5, 3);//投影值统计小于5，长度小于3的区域都为0
    LCDProjHistShow(yhist, 1, 0, 0);//显示y投影图
    //按y方向分析
    for (int y = 0; y < yhist->dataNum; y++)
    {
        //取非0区间的位置
        if (yhist->datap[y] > 0)
        {
            int ry = y;
            while ((ry < yhist->dataNum)&&(yhist->datap[ry]))//直到碰到下一个0，可获得非0片段
            {
                ry++; 
            }
            //截取横向图片
            CVIMAGE ySegImg = YMCV_Creat_ImgCopyTo(myImage2, 0, y, myImage2->width, ry - y);
            LCD_show_image(stx + 0, sty, ySegImg);

            sprintf(myInform, "Y Seg:");
            LCD_ShowString(0, sty, myInform);
            Delay(500);
						
            //x轴投影分析
            CVVECTORS_U16 xhist = YMCV_Binary_XY_Axis_Proj(ySegImg, 0);//投影到x轴
            YMCV_Binary_Proj_Hist_Seg(xhist, 3, 5);//对投影直方图进行分割
            LCDProjHistShow(xhist, 0, stx + 0, sty + ySegImg->height);//显示投影图

            sprintf(myInform, "X Seg:");
            LCD_ShowString(0, sty + ySegImg->height, myInform);
            Delay(500);
            //按x方向分析
            for (int x = 0; x < xhist->dataNum; x++)
            {
                //取非0区间的位置
                if (xhist->datap[x] > 0)
                {
                    int rx = x;
                    while ((rx < xhist->dataNum) && (xhist->datap[rx]))
                    {
                        rx++;
                    }
                    //截取纵向图片
                    CVIMAGE xSegImg = YMCV_Creat_ImgCopyTo(ySegImg, x, 0, rx - x, ySegImg->height);
                    LCD_show_image(stx + x, sty + ySegImg->height*2, xSegImg);
										/*-------------------------- 数字特征提取--------------------------------------------------*/
                    //将图像变为16×16大小
                    CVIMAGE resultImg = YMCV_Binary_MaxPooling_ScaleSizeTo_MxM(xSegImg, 16);
                    LCD_show_image(120, 200, resultImg);
                    CVVECTORS_U16 numberhist = YMCV_Binary_Number16x16_ProjHist16(resultImg);//1x16
										LCD_Fill_ConstRgbRect(200,180,16,60,(color24_t){128,128,128});//清除区域
                    LCDProjHistShow(numberhist, 0, 200, 180);//显示投影特征图
                    sprintf(myInform, "16x1");
                    LCD_ShowString(200, 160, myInform);
                    //强制转为16x1
                    numberhist->dim = numberhist->dataNum;
                    numberhist->dataNum = 1;
										
                    uint8 lable;
                    YMCV_DecisionTree_Classify(numberhist, &lable,myID3);//决策树判别
                    sprintf(myInform, "result %d ", lable);
                    LCD_ShowString(0, 200, myInform);

                    YMCV_Free_ImgFree(resultImg);
                    YMCV_Free_Vector_U16_Free(numberhist);
                    /*-----------------------------------------------------------------------------------------------*/  
                    Delay(300);
                    //释放内存
                    YMCV_Free_ImgFree(xSegImg);
                    //跳过已经取过的区间
                    x = rx;
                }
            }
            
						//清除屏幕区域
						LCD_Fill_ConstRgbRect(stx + 0, sty,ySegImg->width,ySegImg->height,(color24_t){255,255,255});
						LCD_Fill_ConstRgbRect(stx + 0, sty + ySegImg->height-1,ySegImg->width,ySegImg->height*2+1,(color24_t){128,128,128});
						//释放截图内存和投影图
            YMCV_Free_ImgFree(ySegImg);
            YMCV_Free_Vector_U16_Free(xhist);
            //跳过已经取过的区间
            y = ry;
        }
    }
    Delay(300);

		//释放模型内存
    YMCV_DecisionTree_Free(myID3);
    //释放内存
    YMCV_Free_Vector_U16_Free(yhist);
		YMCV_Free_Binary_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
}


//数字识别决策树训练，采用递归的方式训练，对栈长有要求，如果数据较多的话建议放到PC端进行训练
//这里直接放在单片机中训练
void NumberDecisionTree_Train()
{
	 //简单的10个数据集，样本越多效果会越好
	 const uint8 labels[10] = {1,3,2,4,5,7,6,8,0,9};//
	 const int16 mydata[10][16]={ // 10个数据，每个数据是16维度的向量
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
	CVVECTORS_U16 myvec = YMCV_Creat_Vector_U16(16, 10);//创建16维向量，一共10个

	for (int i = 0; i < myvec->dataNum; i++)//初始化向量值
	{
		int16* thisvec = (int16*)(myvec->datap + i * myvec->dim);
		//载入数据到向量中
		for (int j = 0; j < myvec->dim;j++)
		{
			thisvec[j]=mydata[i][j];
		}
	}
	 
	 CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//创建ID3决策模型
	 //ID3决策树数据训练
   YMCV_DecisionTree_ID3_Trains(myID3, myvec, (uint8*)labels);
	 //模型测试
	 {
			uint8 label10[10];
			//决策树数据分类
			YMCV_DecisionTree_Classify(myvec, label10,myID3);
		  for(int i = 0;i < 10; i++)
		   printf("原始标签：%d ，分类后的标签：%d\n",labels[i],label10[i]);
	 }
	
	 printf("\n");
	 //保存模型
	 YMCV_DecisionTree_Save(myID3);
	 printf("\n");
   YMCV_Free_Vector_U16_Free(myvec);
	 
   YMCV_DecisionTree_Free(myID3);//释放模型内存
}
