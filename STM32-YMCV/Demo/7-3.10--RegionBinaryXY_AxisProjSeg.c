#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 xy轴投影分割为例，展示了二值图分割的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
//    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
//    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
//		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgb转灰度图

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
    
    /*-------------------------- y,x投影字符分割 --------------------------------------------------*/
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
    //释放内存
    YMCV_Free_Vector_U16_Free(yhist);
		YMCV_Free_Binary_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);

}

