#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 二值图霍夫变换为例，展示了霍夫空间的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 80;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tuxin02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)tuxin02_gray, sizeof(tuxin02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0

	  LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty =  myImage0_gray->height;
        /*--------------------------灰度Sobel边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//二值图
    CV_edge_param myParam = { .EdgeType = CV_Sobel ,.Thresh.sobel.minT = 100 };
    YMCV_Gray_EdgeDetect(myImage0_gray, myImage1, &myParam);

    //显示图片
    LCD_show_image(stx + 0, sty + 0, myImage1);

    //信息显示
    sprintf(myInform, "Edge Check ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    //暂停
    Delay(300);

    /*-------------------------- hough 空间计算--------------------------------------------------*/
    CVIMAGE houghImg = YMCV_Binary_To_Hough(myImage1, 0);//转为hough空间 角度分辨率为 (2^0)°

    //显示霍夫空间
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(houghImg->width, houghImg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(houghImg, myImage2, CV_IGrayToGray);

    //显示图片2
		{
			CV_geom_param stretchParam = {.stretch.Imgin_w= myImage2->width,
															 .stretch.Imgin_h = myImage2->height, 
															 .stretch.out_w = myImage2->width/2,  //缩放到一半大小
															 .stretch.out_h = myImage2->height/2, };
			//进行缩放后显示
			CVGEOM_RENDER stretchRender = YMCV_Creat_GeomRender_Creat(CV_Stretch);	
			YMCV_GeomKernel_Load(stretchRender, &stretchParam);
			CVIMAGE stretchImage = YMCV_Image_Geom_RenderTo(myImage2, stretchRender);														 
														
			LCD_show_image(0, 0, stretchImage);
		  
			//释放临时内存
			YMCV_Free_GEOM_RenderFree(stretchRender);
			YMCV_Free_ImgFree(stretchImage);
		}

    
        //信息显示
    sprintf(myInform, " hough space ");
    LCD_ShowString(0, 0, myInform);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(houghImg);

    Delay(300);
}





