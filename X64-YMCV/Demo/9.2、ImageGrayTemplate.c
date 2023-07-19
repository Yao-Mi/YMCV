#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 模板匹配为例，展示三种方法匹配的结果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图

    /*--------------------------exp指数增强--------------------------------------------------*/
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);

    //重新创建一个通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//指数增强
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };

    YMCV_SingleChannel_MapTo(myRender, &myParam1);//使用参数进行渲染器通道配置、初始化
    YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);

    //截取部分图像
    CVIMAGE imgFind = YMCV_Creat_ImgCopyTo(myImage1, 60, 40, 35, 30);

    //显示图片
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(300, 30, imgFind);

    //信息显示
    sprintf(myInform, "exp指数增强取模板 ");
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);
    /*-------------------------- MAD 模板匹配计算--------------------------------------------------*/

    //平均绝对差法 
    CVrect result = YMCV_Gray_Matching_Template(myImage0, imgFind, CV_Template_MAD, 5, 20); //若返回的宽高是0，则说明不存在

    //显示结果
    color24_t rgbval = { .R = 255 };
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //信息显示
    sprintf(myInform, "   模板 MAD 匹配      ");
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();

    /*-------------------------- SSDA 模板匹配计算--------------------------------------------------*/

    //平均绝对差法 
    result = YMCV_Gray_Matching_Template(myImage0, imgFind, CV_Template_SSDA, 5, 20); //若返回的宽高是0，则说明不存在

    //显示结果
    LCD_show_image(0, 0, myImage0);
    rgbval.G = 255;
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //信息显示
    sprintf(myInform, "   模板 SSDA 匹配      ");
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();

    /*-------------------------- NCC 模板匹配计算--------------------------------------------------*/
    //平均绝对差法 
    result = YMCV_Gray_Matching_Template(myImage0, imgFind, CV_Template_NCC, 5, 20); //若返回的宽高是0，则说明不存在

    //显示结果
    LCD_show_image(0, 0, myImage0);
    rgbval.R = 0;
    LCD_Draw_Rect(result.x, result.y, result.w, result.h, rgbval);
    //信息显示
    sprintf(myInform, "   模板 NCC 匹配      ");
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(imgFind);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





