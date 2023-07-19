#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_MergeCal.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 自动TriMap和手动TriMap为例，展示了分水岭分割的效果
    *
    ***************************************************************************************/
CVIMAGE YMCV_GetTriMap_Demo(CVIMAGE image0);

int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0


    /*-------------------------- 自动创建TriMap --------------------------------------------------*/
    //使用7-3.6例程的方法创建TriMap
    CVIMAGE TriMapMarkers = YMCV_GetTriMap_Demo(myImage0);

    //显示原图
    LCD_show_image(0, 0, myImage0);
    //显示图片trimap
    LCD_show_image(600, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "TriMap图  ");
    LCD_ShowString(700, 310, myInform);

    PAUSE();

    /*-------------------------- 分水岭分割 --------------------------------------------------*/
    YMCV_Gray_Watershed(myImage0, TriMapMarkers);//将Trimap图像分割清晰
     //显示图片
    LCD_show_image(600, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "分割掩码图  ");
    LCD_ShowString(700, 310, myInform);

    CVIMAGE SegImage = YMCV_Image_Mask(TriMapMarkers, myImage0);//分割图当作掩码使用

    //显示图片分割结果
    LCD_show_image(300, 0, SegImage);
    
    //信息显示
    sprintf(myInform, "分割后的原图  ");
    LCD_ShowString(400, 310, myInform);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);

    PAUSE();

    /*-------------------------- 使用键盘和鼠标 手动创建TriMap --------------------------------------------------*/
    // 按c选择是否绘制
    // q,b选择前景点或者背景点，空格退出
    TriMapMarkers = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers);//初始所有点为待标记状态

    LCD_Clear(0);
    //显示原图
    LCD_show_image(0, 0, myImage0);
    sprintf(myInform, "手动绘制前景点和背景 标记 ");
    LCD_ShowString(0, 310, myInform);

    color24_t thiscolor = { .R = 255,.G = 0 };
    char pflg = 0,qhflg=1;//默认先绘制前景点
    int qval= CVTriMapMaxVl,bval= CVTriMapMinVl;
    printf("准备绘制：\n'c':开始/暂停\n'q':选择前景\n'b':选择背景\n空格：结束绘制");
    while (1)
    {
        int px, py,w,h;
        char mchr;
        w = TriMapMarkers->width;
        h = TriMapMarkers->height;
        //从原图上绘制
        if (LCD_GetXY(0, 0, myImage0->width, myImage0->height,&px,&py)&& pflg)
        {
            LCD_Draw_Point(px,  py, thiscolor);
            //标记前景点
            if (qhflg)
            {
                TriMapMarkers->data[py * w + px] = qval;
            }
            //背景点
            else
            {
                TriMapMarkers->data[py * w + px] = bval;
            }
        }
        if (LCD_GetChar(&mchr))
        {
            if (mchr == ' ') { printf("结束绘制\n"); break; }
            if (mchr == 'c')
            {
                pflg = !pflg;
                if (pflg)printf("开始绘制\n"); else printf("暂停\n");
                
            }
            //绘制背景
            if (mchr == 'b')
            {
                qhflg = 0;
                //修改标记值，标记为不同区间,正常+1就可以，这里为了显示效果采用+20
                bval += 20;
                if (bval >= CVTriMapMidle)bval = CVTriMapMinVl;
                printf("背景=%d\n", bval);
                thiscolor.R = 0;
                thiscolor.G += rand() % 255;
                thiscolor.B = 125+ rand() % 128;
            }
            //绘制前景
            if (mchr == 'q')
            {
                qhflg = 1;
                //修改标记值，标记为不同区间,正常-1就可以，这里为了显示效果采用-20
                qval -= 20;
                if (qval <= CVTriMapMidle)qval = CVTriMapMaxVl;
                printf("前景=%d\n", qval);
                thiscolor.R = 125 + rand() % 128;
                thiscolor.G += rand()%255;
                thiscolor.B = 0;
            }
        }
    } 
    //显示图片trimap
    LCD_show_image(600, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "TriMap图  ");
    LCD_ShowString(700, 310, myInform);

    PAUSE();
    /*-------------------------- 分水岭分割 --------------------------------------------------*/
    YMCV_Gray_Watershed(myImage0, TriMapMarkers);//将Trimap图像分割清晰
     //显示图片
    LCD_show_image(600, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "分割掩码图  ");
    LCD_ShowString(700, 310, myInform);


    SegImage = YMCV_Image_Mask(TriMapMarkers, myImage0);//分割图当作掩码使用

    //显示图片分割结果
    LCD_show_image(300, 0, SegImage);

    //信息显示
    sprintf(myInform, "分割后的前景图  ");
    LCD_ShowString(400, 310, myInform);

    //释放内存
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}




//Trimap生成
CVIMAGE YMCV_GetTriMap_Demo(CVIMAGE image0)
{
    /*--------------------------距离变换创建前景图--------------------------------------------------*/
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(image0->width, image0->height, CVBinaryType);

    //创建二值化通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//单通道二值化，一个通道

    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 0,.gray_2tobin.rightThresh = 180 };    //设置渲染器参数
    YMCV_SingleChannel_MapTo(myRender, &myParam);//使用参数进行渲染器通道配置、初始化  

    //图像二值化
    YMCV_Image_ChannelRenderTo(image0, myImage1, myRender);//使用通道渲染器进行渲染图片


    //对二值图进行距离变换
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);

    //对距离图二值化
    myParam.gray_2tobin.leftThresh = 45;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage2, myImage2, myRender);//二值化渲染

    /*--------------------------距离变换创建背景图--------------------------------------------------*/

    //背景粗提取，前景粗背景取反
    YMCV_Image_Binary_Logic_Cal(myImage1, myImage1, CV_Not);//图1 逻辑非  ，输出结果放在图1

    //对二值图进行距离变换
    CVIMAGE myImage3 = YMCV_Binary_Distance_Transform(myImage1);

    //对距离图二值化
    myParam.gray_2tobin.leftThresh = 40;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage3, myImage3, myRender);//二值化渲染

    /*-------------------------- TriMap图创建--------------------------------------------------*/
    //由于使用Chanel渲染器不会改变图像类型，这里将灰度格式强制转为二值图
    myImage2->type = CVBinaryType;
    myImage3->type = CVBinaryType;
    //使用前景图和背景图，创建trimap图
    CVIMAGE TriMapImage = YMCV_BinaryTo_TriMap(myImage2, myImage3);

    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);
    //释放内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    return TriMapImage;
}




