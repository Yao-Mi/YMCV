
/*******************************************************************************************
    *
    * 说明  该例程以 距离变换为例，展示了生成Trimap的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图
		//显示图片0
		LCD_show_image(stx + 0, 0, myImage0_gray);
    /*--------------------------距离变换创建前景图--------------------------------------------------*/

    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);

    //创建二值化通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//单通道二值化，一个通道

    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 130,.gray_2tobin.rightThresh = 255 };    //设置渲染器参数
    YMCV_SingleChannel_MapTo(myRender, &myParam);//使用参数进行渲染器通道配置、初始化  

    //图像二值化
    YMCV_Image_ChannelRenderTo(myImage0_gray, myImage1, myRender);//使用通道渲染器进行渲染图片

    //对二值图进行距离变换
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);
		
    //显示图片2
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);
		//信息显示
	  sprintf(myInform, "front distance Transform");
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//暂停300ms
		Delay(300);
		

    //对距离图二值化
    myParam.gray_2tobin.leftThresh = 20;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage2, myImage2, myRender);//二值化渲染

		//显示图片2
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);
		//信息显示
	  sprintf(myInform, "front thresh:[%3d,%3d] ",myParam.gray_2tobin.leftThresh,myParam.gray_2tobin.rightThresh);
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//暂停300ms
		Delay(300);

    /*--------------------------距离变换创建背景图--------------------------------------------------*/

    //背景粗提取，前景粗背景取反
    YMCV_Image_Binary_Logic_Cal(myImage1, myImage1, CV_Not);//图1 逻辑非 ，输出结果放在图1

    //对二值图进行距离变换
    CVIMAGE myImage3 = YMCV_Binary_Distance_Transform(myImage1);
		
		//显示图片3
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);
		//信息显示
	  sprintf(myInform, "behind distance Transform");
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//暂停300ms
		Delay(300);

    //对距离图二值化
    myParam.gray_2tobin.leftThresh = 40;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage3, myImage3, myRender);//二值化渲染

		//显示图片3
		LCD_show_image(stx + 0, myImage0_gray->height, myImage3);
		//信息显示
	  sprintf(myInform, "behind thresh:[%3d,%3d] ",myParam.gray_2tobin.leftThresh,myParam.gray_2tobin.rightThresh);
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//暂停300ms
		Delay(300);

    /*-------------------------- TriMap图创建--------------------------------------------------*/
    //将灰度格式强制转为二值图
    myImage2->type = CVBinaryType;
    myImage3->type = CVBinaryType;
    //使用前景图和背景图，创建trimap图
    CVIMAGE TriMapImage = YMCV_BinaryTo_TriMap(myImage2, myImage3);

    //显示图片trimap
		LCD_show_image(stx + 0, myImage0_gray->height, TriMapImage);

    //信息显示
    sprintf(myInform, "TriMap image creat        ");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		//暂停800ms
		Delay(800);
		
    //通道渲染器内存释放
    YMCV_Free_PIX_RenderFree(myRender);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(TriMapImage);
}









