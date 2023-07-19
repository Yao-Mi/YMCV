#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FormCge.h"
#include"../YMCV/CORE/YMCV_FreqCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 图像dct为例，展示了将一幅图像变成陈旧图像的效果
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图

    myData = CV_malloc1(sizeof(mm00));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00, sizeof(mm00));
    CVIMAGE myImage4 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVRgb16Type);//灰度图
    //显示原图
    LCD_show_image(0, 0, myImage4);

    //创建DCT渲染器
    uint8 kernel_r = 0;
    CVDCT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//设置填充值

    /*--------------------------灰度图像dct变换--------------------------------------------------*/

    //创建待处理图像的专属 dct滤波核
    myKernel = YMCV_Creat_DCT2D_Render_Creat(myImage0->width, myImage0->height);
    //使用滤波器创建dct输出图像，最好使用滤波器计算后的参数来创建,类型必须float类型
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myKernel->width, myKernel->height, CVFGrayType);
    //进行DCT变换
    YMCV_Gray_Image_DCT(myKernel, myImage0, myImage1);//结果存在myImage1

    /*--------------------------频域图像显示--------------------------------------------------*/
   //显示频谱幅度图片2
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    float32* dap = myImage1->data;

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

    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "dct 变换后结果图"); //信息显示
    LCD_ShowString(100, 510, myInform);
    PAUSE();

    /*--------------------------频域图像idct变换--------------------------------------------------*/

    //创建idct输出的结果图像，与原始图一致即可
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    //进行iFFT变换
    YMCV_Gray_Image_IDCT(myKernel, myImage3, myImage1);//结果存回myImage3
    //显示图片4
    YMCV_Gray_ColorTo_Rgb16(myImage3, myImage4);//将灰度染到原彩色图上
    LCD_show_image(500, 0, myImage4);
    sprintf(myInform, "受污效果"); //信息显示
    LCD_ShowString(100, 510, myInform);


    //释放DCT渲染器
    YMCV_Free_DCT2D_RenderFree(myKernel);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage3);

    PAUSE();
    LCD_Destory();
    return 0;
}

