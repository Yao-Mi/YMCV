#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 图像dct为例，展示了将一幅图像变成陈旧图像的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图

    myData = CV_malloc1(sizeof(mm02));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02, sizeof(mm02));
    CVIMAGE myImage4 = YMCV_Creat_DataToImg((uint8*)myData,160, 120, CVRgb16Type);//灰度图
    //显示原图
	  LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;
    //创建DCT渲染器
    uint8 kernel_r = 0;
    CVDCT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//设置填充值

    /*--------------------------灰度图像dct变换--------------------------------------------------*/

    //创建待处理图像的专属 dct滤波核
    myKernel = YMCV_Creat_DCT2D_Render_Creat(myImage0_gray->width, myImage0_gray->height);
    //使用滤波器创建dct输出图像，最好使用滤波器计算后的参数来创建,类型必须float类型
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myKernel->width, myKernel->height, CVFGrayType);
    //进行DCT变换
    YMCV_Gray_Image_DCT(myKernel, myImage0_gray, myImage1);//结果存在myImage1

    /*--------------------------频域图像显示--------------------------------------------------*/
   //显示频谱幅度图片2
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    float32* dap = (float32*)myImage1->data;

    //频谱图像过滤
    for (int i = 0; i < myImage1->height; i++)
    {
        for (int j = 0; j < myImage1->width; j++)
        {
            if ((dap[i * myImage1->width + j]) < 10)
            {
                dap[i * myImage1->width + j] = 0;
            }
        }
    }
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_FGrayToGray);

    LCD_show_image(stx + 0, sty, myImage2);
    sprintf(myInform, "dct result"); //信息显示
    LCD_ShowString(stx + 0, sty, myInform);
    Delay(300);

    /*--------------------------频域图像idct变换--------------------------------------------------*/

    //创建idct输出的结果图像，与原始图一致即可
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    //进行iFFT变换
    YMCV_Gray_Image_IDCT(myKernel, myImage3, myImage1);//结果存回myImage3
    //显示图片4
    LCD_show_image(stx + 0, 0, myImage4);
    Delay(200);
		//将灰度染到原彩色图上
    YMCV_Gray_ColorTo_Rgb16(myImage3, myImage4);
		//受污效果
    LCD_show_image(stx + 0, sty, myImage4);
    sprintf(myInform, "polluting effects"); //信息显示
    LCD_ShowString(stx + 0, sty, myInform);

    Delay(500);
    //释放空间渲染器
    YMCV_Free_DCT2D_RenderFree(myKernel);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(myImage4);
}

