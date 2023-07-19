


#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_Special_Effect.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include"../YMCV/IOFILE/YMCV_IOFile.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 标定数据为例，展示了人脸特征点数据标定和提取
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 600);

    CVIMAGE calibrat = YMCV_Image_Read("D:/Documents/1012_YMCV/图片素材/GAN-Face/template.jpg", CVRgb16Type);
    //显示原图
    LCD_show_image(400, 0, calibrat);

    sprintf(myInform, "手动标定特征点 ");
    LCD_ShowString(0, 310, myInform);
    /*-------------------------- 手动标定 --------------------------------------------------*/

    color24_t thiscolor = { .R = 255,.G = 0 };
    LCD_Point_Color_Set(thiscolor);
    char pflag = 1;
    printf("准备绘制：\n'c':选择\n'z'后退\n'n'下一张\n空格：结束绘制\n");
    int px = 0, py = 0, w, h;

    uint16 xpoint[100], ypoint[100];
    int8 num = -1,readimgi = 0;
    //读取一副图片
    CVIMAGE PrImg = YMCV_Image_Read("D:/Documents/1012_YMCV/图片素材/GAN-Face/0.jpg", CVRgb16Type);

    while (1)
    {
        char mchr;
       
        //获取按键值
        if (LCD_GetChar(&mchr))
        {
            
            if (mchr == 'c')
            {
                num++;
                if (LCD_GetXY(0, 0, PrImg->width, PrImg->height, &px, &py))
                {
                    xpoint[num] = px;
                    ypoint[num] = py;
                    printf("%02dc选择点：%d , %d\n",num + 1, px, py);
                }
                pflag = 1;
            }
            else if (mchr == 'z')
            {
                //后退一步
                num--;
                if (num < 0)num = -1;
                pflag = 1;
            }
            else if ((mchr == 'n') && ((num + 1) == 40))//记录完40个特征点
            {
                FILE* recordfile = fopen("D:/Documents/1012_YMCV/图片素材/GAN-Face/CalibratingData.txt", "a");//以追加写入方式打开
                uint8 databuff[20];
                //特征点保存
                for (int i = 0; i <= num; i++)
                {
                    sprintf(databuff, " %3d %3d", xpoint[i], ypoint[i]);//转文本
                    fwrite(databuff, strlen(databuff), 1, recordfile);//写入
                }
                fwrite("\n", 1, 1, recordfile);//写入换行
                fclose(recordfile);//关闭文件

                //下一幅图片
                readimgi++;
                if (readimgi >= 30)//30个样本全部标注完了
                {
                    break;
                }
                sprintf(myInform, "D:/Documents/1012_YMCV/图片素材/GAN-Face/%d.jpg", readimgi);
                LCD_Fill_ConstRgbRect(0, 0, PrImg->width, PrImg->height, (color24_t){ 0 });//清除区域
                //释放之前内存
                YMCV_Free_ImgFree(PrImg);
                //读取一副图片
                PrImg = YMCV_Image_Read(myInform, CVRgb16Type);
                num = -1;
                pflag = 1;
                system("cls");
                printf("准备绘制：\n'c':选择\n'z'后退\n'n'下一张\n空格：结束绘制\n");
            }
        }
        if (num >= 100) break;

        //重绘图形
        if (pflag)
        {
            LCD_show_image(0, 0, PrImg);
            //点
            for (int i = 0; i <= num; i++)
            {
                //从原图上绘制
                LCD_Fill_ConstRgbRect(xpoint[i] - 1, ypoint[i] - 1, 3, 3, thiscolor);
            }
            pflag = 0;
        }
    }

    //释放内存
    YMCV_Free_ImgFree(PrImg);
    YMCV_Free_ImgFree(calibrat);

    PAUSE();
    LCD_Destory();
    return 0;
}

