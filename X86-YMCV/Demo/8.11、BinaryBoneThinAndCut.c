#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 二值图骨架细化 和剪枝 为例，展示了细化和剪枝效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(guajia_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, guajia_gray, sizeof(guajia_gray));
    //二值图
    for (int i = 0; i < sizeof(guajia_gray); i++)
    {
        myData[i] = (myData[i] < 127) ? 255 : 0;//快速二值化
    }
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 300, 300, CVBinaryType);//灰度图

    /*-------------------------- 形态学操作 --------------------------------------------------*/
    CV_kernel_1param myParam;//最大值滤波无需参数加载
    YMCV_PADDING_VAL = 0;//设置边界填充值为0
    //极大值滤波，二值图膨胀效果
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 1, OutPaddingConst);//创建相应半径的滤波核
    YMCV_Space_SingleKernel_Load(myKernel, &myParam); //使用参数进行渲染器通道配置、初始化
    CVIMAGE myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);//使用空间渲染器进行渲染灰度图片


    //显示图片
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(300, 0, myImage1);

    //信息显示
    sprintf(myInform, " 形态学膨胀修复小孔.");
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();

    /*--------------------------骨架细化计算--------------------------------------------------*/
    LCD_show_image(0, 0, myImage1);//原图显示在左边

    YMCV_Image_Binary_Bone_Thin(myImage1);

    LCD_show_image(300, 0, myImage1);//处理后显示在右边
    //信息显示
    sprintf(myInform, "      二值图骨架细化     ");
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();

    /*--------------------------骨架剪枝计算--------------------------------------------------*/
    LCD_show_image(0, 0, myImage1);//原图显示在左边

    YMCV_Image_Binary_Bone_Cut(myImage1, 5);//长度小于5的毛刺都剔除

    LCD_show_image(300, 0, myImage1);//处理后显示在右边

        //信息显示
    sprintf(myInform, "      二值图骨架剪枝     ");
    LCD_ShowString(100, 300, myInform);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





