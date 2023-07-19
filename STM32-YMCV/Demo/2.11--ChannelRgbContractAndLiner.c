
/*******************************************************************************************
    *
    * 说明  该例程以对比度、分段线性增强为例，展示了参数取值 、增长步幅，以及通道转换示意图的显示
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    LCD_Clear(0);
	  //显示图片0
    LCD_show_image(stx + 0, 0, myImage0);

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);

    /*--------------------------彩色图对比度增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Contract, PIXChannelNum3);//图像对比度增强/减弱（雾化）
    //设置渲染器参数
    CV_pixchannel_3param myParam = { .rgb_contract.R_k = -100.0f, .rgb_contract.G_k = -100.0f, .rgb_contract.B_k = -100.0f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam);
        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
			
			  //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
			  //显示通道转换图
        LCDChannelTransformShow(myRender, 170, 220, 40);
			
        //信息显示
			  sprintf(myInform, "contract:\n R%01.2f G%01.2f B%01.2f  ",
              			myParam.rgb_contract.R_k, myParam.rgb_contract.G_k, myParam.rgb_contract.B_k);
        LCD_ShowString(0, 0, myInform);
			 
        //暂停200ms
        Delay(200);

        //参数调整
        myParam.rgb_contract.R_k += 20.0f;
        myParam.rgb_contract.G_k += 15.0f;
        myParam.rgb_contract.B_k += 10.0f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------彩色图分段线性增强--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Liner_Enhance, PIXChannelNum3);//分段线性增强
    //设置渲染器参数 将[0,f_a][f_a,f_b][f_b,f_c][f_c,255] 变换为 [0,t_a][t_a,t_b][t_b,t_c][t_c,255] 
    CV_pixchannel_3param myParam1 = { .rgb_liner_enhance.R_from_a = 20,.rgb_liner_enhance.R_from_b = 100,.rgb_liner_enhance.R_from_c = 200,//R参数
                                      .rgb_liner_enhance.R_to_a = 30, .rgb_liner_enhance.R_to_b = 30, .rgb_liner_enhance.R_to_c = 230 ,
                                      .rgb_liner_enhance.G_from_a = 20,.rgb_liner_enhance.G_from_b = 100,.rgb_liner_enhance.G_from_c = 200,//G参数
                                      .rgb_liner_enhance.G_to_a = 30, .rgb_liner_enhance.G_to_b = 30, .rgb_liner_enhance.G_to_c = 230 ,
                                      .rgb_liner_enhance.B_from_a = 20,.rgb_liner_enhance.B_from_b = 100,.rgb_liner_enhance.B_from_c = 200,//B参数
                                      .rgb_liner_enhance.B_to_a = 30, .rgb_liner_enhance.B_to_b = 30, .rgb_liner_enhance.B_to_c = 230 , };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        
			  //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
			  //显示通道转换图
        LCDChannelTransformShow(myRender, 170, 220, 40);

        //信息显示
			  sprintf(myInform, "liner RGB:");
			      LCD_ShowString(0, 0, myInform);
        sprintf(myInform, "R[%03d %03d %03d]->[%03d %03d %03d]",
            myParam1.rgb_liner_enhance.R_from_a, myParam1.rgb_liner_enhance.R_from_b, myParam1.rgb_liner_enhance.R_from_c, \
            myParam1.rgb_liner_enhance.R_to_a, myParam1.rgb_liner_enhance.R_to_b, myParam1.rgb_liner_enhance.R_to_c); \
            LCD_ShowString(0, 0 + 16, myInform);
        sprintf(myInform, "G[%03d %03d %03d]->[%03d %03d %03d]",
            myParam1.rgb_liner_enhance.G_from_a, myParam1.rgb_liner_enhance.G_from_b, myParam1.rgb_liner_enhance.G_from_c, \
            myParam1.rgb_liner_enhance.G_to_a, myParam1.rgb_liner_enhance.G_to_b, myParam1.rgb_liner_enhance.G_to_c); \
            LCD_ShowString(0, 0 + 16*2, myInform);
        sprintf(myInform, "B[%03d %03d %03d]->[%03d %03d %03d]",
            myParam1.rgb_liner_enhance.B_from_a, myParam1.rgb_liner_enhance.B_from_b, myParam1.rgb_liner_enhance.B_from_c, \
            myParam1.rgb_liner_enhance.B_to_a, myParam1.rgb_liner_enhance.B_to_b, myParam1.rgb_liner_enhance.B_to_c); \
            LCD_ShowString(0, 0 + 16*3, myInform);
        //暂停200ms
        Delay(200);

        //参数调整 一般选择好固定的from参数后，调节to参数，所有参数范围是[0 -- 255]
        myParam1.rgb_liner_enhance.R_to_b += 20;//[to_a -- to_c]
        myParam1.rgb_liner_enhance.G_to_b += 15;
        myParam1.rgb_liner_enhance.B_to_b += 10;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
}





