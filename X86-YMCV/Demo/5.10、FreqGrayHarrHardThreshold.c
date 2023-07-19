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
    * 说明  该例程以 图像dwt为例，展示了将一幅图像进行dwt变换及其逆变换的结果，
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
    //显示原图
    LCD_show_image(0, 0, myImage0);

    //创建空间渲染器
    uint8 kernel_r = 0;
    CVDWT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//设置填充值

    /*--------------------------灰度图像dwt变换--------------------------------------------------*/

    //创建待处理图像的专属 dwt滤波核
    myKernel = YMCV_Creat_DWT2D_Render_Creat(myImage0->width, myImage0->height);
    //使用滤波器创建dwt输出图像，必须使用滤波器计算后的参数来创建,以满足滤波器需求,类型必须float类型
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myKernel->width, myKernel->height, CVFGrayType);
    //进行DWT变换
    YMCV_Gray_Image_DWT(myKernel, myImage0, myImage1);//结果存在myImage1

    /*--------------------------变换图像结果展示--------------------------------------------------*/
    //显示频谱幅度图片2
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_FGrayToGray);

    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "dwt 变换后的结果图"); //信息显示
    LCD_ShowString(100, 510, myInform);

    //暂停
    PAUSE();
    /*--------------------------频域图像滤波--------------------------------------------------*/
    //创建哈尔小波硬阈值波器
    CVFREQ_RENDER freqKernl = YMCV_Creat_FreqKernel_Render_Creat(CV_HarrThreshFilter, 0);
    //加载滤波器参数
    CV_freq_1param myparam = { .harr.threshold= 45 };
    YMCV_Freq_Kernel_Load(freqKernl, myparam);
    //对幅度进行滤波
    YMCV_Gray_Image_Freq_Kernel_Render(myImage1, freqKernl);

    /*--------------------------图像idwt变换--------------------------------------------------*/
     //创建idwt输出的结果图像，与原始图一致即可
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    //进行iDWT变换
    YMCV_Gray_Image_IDWT(myKernel, myImage3, myImage1);//结果存回myImage3
    //显示图片2
    LCD_show_image(500, 0, myImage3);
    sprintf(myInform, "idwt 变换后的结果图"); //信息显示
    LCD_ShowString(100, 510, myInform);


    //释放DWT渲染器
    YMCV_Free_DWT2D_RenderFree(myKernel);
    YMCV_Free_FreqKernel_RenderFree(freqKernl);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);

    PAUSE();
    LCD_Destory();
    return 0;
}








