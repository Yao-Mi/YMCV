#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FormCge.h"
#include"../YMCV/CORE/YMCV_FreqCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 图像fft为例，展示了将一幅图像进行带通滤波的结果，
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
    CVFFT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//设置填充值

    /*--------------------------图像fft变换及shift变换--------------------------------------------------*/

    //创建待处理图像的专属 fft滤波核
    myKernel = YMCV_Creat_FFT2D_Render_Creat(myImage0->width, myImage0->height);
    //使用滤波器创建fft输出图像，最好使用滤波器计算后的参数来创建,类型必须complex类型
    //当然，若不使用滤波器的宽度和高度进行创建，图像也会自动重新规划大小，以满足滤波器需求
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVCGrayType);
    //进行FFT变换
    YMCV_Gray_Image_FFT(myKernel, myImage0, myImage1);//结果存在myImage1

    //fftshift变换
    YMCV_CGray_Image_FFTshift(myImage1);

    //显示频谱图片2
    YMCV_CGray_Image_LogMag_AndPhase(myImage1);//先转幅度和相位
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_CGrayToGray2);

    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "fft 变换后shift结果图"); //信息显示
    LCD_ShowString(100, 510, myInform);

    //暂停
    PAUSE();

    /*--------------------------频域图像滤波--------------------------------------------------*/
    //创建高斯高通滤波器,复数格式为幅度与相位
    CVFREQ_RENDER freqKernl = YMCV_Creat_FreqKernel_Render_Creat(CV_BandStopFilter, CV_RhoAndTheta);
    //加载滤波器参数
    CV_freq_1param myparam = { .band.D0 = 50,.band.W = 45 };
    YMCV_Freq_Kernel_Load(freqKernl, myparam);
    //对幅度进行滤波
    YMCV_Gray_Image_Freq_Kernel_Render(myImage1, freqKernl);

    //显示滤波后的结果
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_CGrayToGray2);

    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "带通滤波后的结果图"); //信息显示
    LCD_ShowString(100, 510, myInform);

    //暂停
    PAUSE();
    /*--------------------------频域图像ifftshift变换与ifft变换--------------------------------------------------*/
    YMCV_CGray_Image_expMag_ToComplex(myImage1);//幅频图转复数图
    //ifftshift变换 对于偶数宽高图像，shift与ishift操作相同
    YMCV_CGray_Image_FFTshift(myImage1);


    //创建ifft输出的结果图像，与原始图一致即可
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    //进行iFFT变换
    YMCV_Gray_Image_IFFT(myKernel, myImage3, myImage1);//结果存回myImage3
    //显示图片2
    LCD_show_image(500, 0, myImage3);
    sprintf(myInform, "ifft 变换结果图           "); //信息显示
    LCD_ShowString(100, 510, myInform);

    //释放FFT渲染器
    YMCV_Free_FFT2D_RenderFree(myKernel);
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








