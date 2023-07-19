#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SpaceCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 FishEye为例，展示了鱼眼模拟及矫正效果
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1100, 700);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(answerTmp_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, answerTmp_gray, sizeof(answerTmp_gray));
    CVIMAGE tempImg = YMCV_Creat_DataToImg((uint8*)myData, 500, 647, CVGrayType);//灰度图0

    uint8* myData2 = CV_malloc1(sizeof(answer1_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData2, answer1_gray, sizeof(answer1_gray));
    CVIMAGE as1Img = YMCV_Creat_DataToImg((uint8*)myData2, 500, 647, CVGrayType);//灰度图0

    //显示原图
    LCD_show_image(0, 0, tempImg);
    LCD_show_image(510, 0, as1Img);
    //信息显示
    sprintf(myInform, "原图  ");
    LCD_ShowString(0, 0, myInform);
    PAUSE();
    /*-------------------------- 二值化 --------------------------------------------------*/
     //创建输出二值图像
    CVIMAGE Tmpbin = YMCV_Creat_Img_Creat(tempImg->width, tempImg->height, CVBinaryType);//二值图
    CVIMAGE as1bin = YMCV_Creat_Img_Creat(as1Img->width, as1Img->height, CVBinaryType);//二值图
    //进行二值化
    CV_binary_param myParam = { .gray_double_t.thresh_min = 0 ,.gray_double_t.thresh_max = 170 };//取0-80区间
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold); //创建二值变换渲染器
    YMCV_BinaryKernel_Load(myRender1, &myParam); //加载参数进行渲染配置、初始化

    //将目标图像的进行渲染
    YMCV_Image_ThreshTo_Binary(tempImg, Tmpbin, myRender1);
    YMCV_Image_ThreshTo_Binary(as1Img, as1bin, myRender1);

    //显示二值图
    LCD_show_image(0, 0, Tmpbin);
    LCD_show_image(510, 0, as1bin);
    //信息显示
    sprintf(myInform, "二值图  ");
    LCD_ShowString(0, 0, myInform);
    PAUSE();

    /*-------------------------- 和模板图做异或 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(Tmpbin, as1bin, CV_And);// myImage3 = myImage3 & myImage2

    LCD_show_image(510, 0, as1bin);
    //信息显示
    sprintf(myInform, " 逻辑与  ");
    LCD_ShowString(510, 0, myInform);
    PAUSE();

    /*-------------------------- 形态学开运算 --------------------------------------------------*/

    CVKERN_RENDER minKernel, maxKernel;
    YMCV_PADDING_VAL = 0;//设置填充值

    //加载渲染器参数，便于进行初始化
    CV_kernel_1param morpParam;//极大、极小滤波无需加载
    minKernel = YMCV_Creat_SpaceRender_Creat(CV_MinFilter, PIXChannelNum1, 1, OutPaddingConst);//创建半径为3的滤波核
    YMCV_Space_SingleKernel_Load(minKernel, &morpParam);//使用参数进行渲染器通道配置、初始化，最小值滤波无需加载，该步骤也可省略
    //使用空间渲染器进行渲染灰度图片
    CVIMAGE as1bin_min = YMCV_Image_Space_SigleKernel_Render(as1bin, minKernel);

    LCD_show_image(510, 0, as1bin_min);
    //信息显示
    sprintf(myInform, " 形态学腐蚀  ");
    LCD_ShowString(510, 0, myInform);

    PAUSE();

    maxKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 1, OutPaddingConst);//创建半径为3的滤波核
    YMCV_Space_SingleKernel_Load(maxKernel, &morpParam);//使用参数进行渲染器通道配置、初始化，最小值滤波无需加载，该步骤也可省略
    //使用空间渲染器进行渲染灰度图片
    CVIMAGE as1bin_max = YMCV_Image_Space_SigleKernel_Render(as1bin_min, maxKernel);

    LCD_show_image(510, 0, as1bin_max);
    //信息显示
    sprintf(myInform, " 形态学膨胀  ");
    LCD_ShowString(510, 0, myInform);

    PAUSE();
    /*-------------------------- 区域个数提取 --------------------------------------------------*/
    CVcolor minVal = { .gray = 150 }, maxVal = {.gray =255};
    CVVECTORS_U16 blobs = YMCV_Image_ColorBox_Find(as1bin_max, minVal, maxVal, 10, 1);

    //LCD_show_image(510, 0, as1bin_max);
    LCD_show_image(510, 0, as1Img);
    LCD_show_image(0, 0, tempImg);

    //显示blob的box
    color24_t thiscolor = { .R = 255 };
    LCD_Point_Color_Set(thiscolor);
    int score = 0;
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        //LCD_Draw_Rect(510 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
        //打勾?
        int x1 = 510 + blobp->box.x - 10;
        int x2 = 510 + blobp->box.x + 5;
        int y1 = blobp->box.y;
        int y2 = blobp->box.y + 8;
        LCD_Draw_Line(x1, y1, x2, y2);
        x1 += 30;
        y1 -= 15;
        LCD_Draw_Line(x2, y2, x1, y1);
        score++;//得分统计
    }
    //信息显示
    sprintf(myInform, "答对了 %d 道题  ", score);
    LCD_ShowString(680, 50, myInform);

    PAUSE();
    //释放内存
    YMCV_Free_ImgFree(as1Img);
    YMCV_Free_ImgFree(tempImg);
    YMCV_Free_ImgFree(as1bin);
    YMCV_Free_ImgFree(Tmpbin);
    YMCV_Free_ImgFree(as1bin_min);
    YMCV_Free_ImgFree(as1bin_max);
    YMCV_Free_KERN_RenderFree(minKernel);
    YMCV_Free_KERN_RenderFree(maxKernel);
    PAUSE();
    LCD_Destory();
    return 0;
}

