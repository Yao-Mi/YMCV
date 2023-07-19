#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 二值图骨架细化 和剪枝 为例，展示了细化和剪枝效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(gujia1_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)gujia1_gray, sizeof(gujia1_gray));
    //二值图
    for (int i = 0; i < sizeof(gujia1_gray); i++)
    {
        myData[i] = (myData[i] < 127) ? 255 : 0;//快速二值化
    }
    CVIMAGE myImage0b = YMCV_Creat_DataToImg((uint8*)myData, 120, 120, CVBinaryType);//二值图

    LCD_show_image(stx + 0, 0, myImage0b);
		int sty = myImage0b->height;
    /*-------------------------- 形态学膨胀修复小孔 --------------------------------------------------*/
    CV_kernel_1param myParam;//最大值滤波无需参数加载
    YMCV_PADDING_VAL = 0;//设置边界填充值为0
    //极大值滤波，二值图膨胀效果
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 1, OutPaddingConst);//创建相应半径的滤波核
    YMCV_Space_SingleKernel_Load(myKernel, &myParam); //使用参数进行渲染器通道配置、初始化
    CVIMAGE myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0b, myKernel);//使用空间渲染器进行渲染灰度图片


    //显示图片
    LCD_show_image(stx + 0, sty, myImage1);

    //信息显示
    sprintf(myInform, "Morp Dilation");
    LCD_ShowString(stx + 0, sty, myInform);
    //暂停
    Delay(500);

    /*--------------------------骨架细化计算--------------------------------------------------*/
		
    YMCV_Image_Binary_Bone_Thin(myImage1);

    LCD_show_image(stx + 0, sty, myImage1);
    //信息显示
    sprintf(myInform, "Binary Thin ");
    LCD_ShowString(stx + 0, sty, myInform);
    //暂停
    Delay(500);

    /*--------------------------骨架剪枝计算--------------------------------------------------*/

    YMCV_Image_Binary_Bone_Cut(myImage1, 5);//长度小于5的毛刺都剔除

    LCD_show_image(stx + 0, sty, myImage1);//处理后显示在右边

        //信息显示
    sprintf(myInform, " Bone Cut  ");
    LCD_ShowString(stx + 0, sty, myInform);


		//释放空间渲染器
		YMCV_Free_KERN_RenderFree(myKernel);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0b);
    YMCV_Free_ImgFree(myImage1);

    Delay(500);
}





