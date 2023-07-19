

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
    * 说明  该例程以 人脸特征点为例，展示了特征点学习效果
    *
    ***************************************************************************************/

#include "../YMCV/CORE/YMCV_FastMath.h"
#include "../YMCV/CORE/YMCV_MergeCal.h"

#include"../YMCV/CORE/YMCV_FormCge.h"
#include"../YMCV/CORE/YMCV_MatchingCal.h"

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 600);
    uint16 movex = 50, movey = 50;//平移量
    CVIMAGE Backimg = YMCV_Creat_Img_Creat(350, 350,CVGrayType);
    CVIMAGE calibrat = YMCV_Image_Read("D:/Documents/1012_YMCV/图片素材/GAN-Face/template.jpg", CVRgb16Type);
    //显示原图
    LCD_show_image(0, 0, Backimg);
    LCD_show_image(400, 0, calibrat);

    sprintf(myInform, "读取标记特征 ");
    LCD_ShowString(0, 310, myInform);

    /*-------------------------- 读取并显示特征标记 --------------------------------------------------*/

    color24_t thiscolor = { .R = 0,.G = 255 };//绿色
    LCD_Point_Color_Set(thiscolor);

    thiscolor = (color24_t){ .R = 255 };//红色
    
    FILE* recordfile = fopen("D:/Documents/1012_YMCV/图片素材/GAN-Face/CalibratingData_ok.txt", "r");//以只读方式打开
    uint8 databuff[400];
    uint16 outdi = 0, fupNum = 30;
    uint16 outdata[200] = {0};
    //40个特征点，缓存队列
    CVVECTORS_F32 Xpts = YMCV_Creat_Vector_F32(40 * sizeof(CVfpoint) / sizeof(float32), fupNum);

    
    //读取30行特征点
    for (int i = 0; i < fupNum; i++)
    {
        sprintf(myInform, "D:/Documents/1012_YMCV/图片素材/GAN-Face/%d.jpg", i);
        //读取一副图片
        CVIMAGE PrImg = YMCV_Image_Read(myInform, CVGrayType);

        YMCV_Image_Cover(PrImg, Backimg, movex, movey);//合并到底色图片中

        LCD_show_image(0, 0, Backimg);
        //从标记数据文本中获取数据
        fgets(databuff, sizeof(databuff), recordfile);//读取一行
        //文本转数据
        char* sptr = databuff;
        outdi = 0;
        while (sptr != NULL)
        {
            if ((*sptr) == '\n')//行尾结束
                break;
            outdata[outdi++] = strtol(sptr, &sptr, 10) + movex;
            outdata[outdi++] = strtol(sptr, &sptr, 10) + movey;
        }

        //显示
        LCD_FaceFeaturePoint_Show(outdata, 0, 0);
        //记录
        CVfpoint* fup = &Xpts->datap[i * Xpts->dim];
        for (int pi = 0; pi < 40; pi++)//40个特征点
        {
            fup[pi].x = outdata[2 * pi + 0];
            fup[pi].y = outdata[2 * pi + 1];
        }
        //PAUSE();
        //Delay(20);
        //释放之前内存
        YMCV_Free_ImgFree(PrImg); 
    }
    fclose(recordfile);//关闭文件

    /*-------------------------- 特征训练 --------------------------------------------------*/

    CV_facefpoint_train mybase = YMCV_FacePoint_Train_Bases(Xpts);//获得数据特征（基向量和投影方差）

    //获得特征描述
    for (int i = 0; i < fupNum; i++)//读取30幅图像
    {
        sprintf(myInform, "D:/Documents/1012_YMCV/图片素材/GAN-Face/%d.jpg", i);
        //读取一副图片
        CVIMAGE PrImg = YMCV_Image_Read(myInform, CVGrayType);
        YMCV_Image_Cover(PrImg, Backimg, movex, movey);//合并到底色图片中，这边要和之前读取的时候一致

        //对图像进行变换，和标准模型齐
        float32 matdata[9] = { 0.0f };
        //计算变换矩阵
       // a -b  0   1  0  -cx    a -b  -(a*cx - b*cy)
       // b  a  0 * 0  1  -cy =  b  a  -(b*cx + a*cy)
       // 0  0  1   0  0   1     0  0   1
       float32* alm = &mybase.alignMat->datap[i * mybase.alignMat->col];
       matdata[0] = alm[0]; matdata[1] = alm[1];
       matdata[3] = alm[3]; matdata[4] = alm[4];
       matdata[8] = 1.0f;
       float cx = alm[2], cy = alm[5];
       matdata[2] = -(alm[0] * cx + alm[1] * cy);
       matdata[5] = -(alm[3] * cx + alm[4] * cy);

       CVfmat tmat = { .row = 3,.col = 3,.datap = matdata };
       //缩放
       //s  0  0   a -b  tx    s*a -b   tx
       //0  s  0 * b  a  ty =   b  s*a  ty
       //0  0  1   0  0  1      0   0   1
       tmat.datap[0] *= mybase.scale;//x轴扩大
       tmat.datap[4] *= mybase.scale;//y
       //偏移回中心
       //1  0  cx   a -b  tx    a -b  (tx + cx)
       //0  1  cy * b  a  ty =  b  a  (tx + cy)
       //0  0   1   0  0  1      0  0   1
       //tmat.datap[2] += cx;
       //tmat.datap[5] += cy;

       //矩阵转为几何变换渲染器
       CVGEOM_RENDER geoHrender = YMCV_HomographyMat_ToPerspective_GeomRender(&tmat, Backimg->width, Backimg->height, CV_WinSize_Const);//固定窗口
       CVIMAGE myImage2 = YMCV_Image_Geom_RenderTo(Backimg, geoHrender);//进行渲染

       LCD_show_image(0, 0, myImage2);//显示
        //特征点进行变换
       uint16 xypoints[40 * 2];
       CVfpoint* fup = &Xpts->datap[i * Xpts->dim];
       for (int pi = 0; pi < 40; pi++)//40个特征点
       {
           xypoints[2 * pi + 0] = matdata[0] * fup[pi].x + matdata[1] * fup[pi].y + matdata[2];
           xypoints[2 * pi + 1] = matdata[3] * fup[pi].x + matdata[4] * fup[pi].y + matdata[6];
       }
       //patch训练
       YMCV_FacePoint_Train_Patch(&mybase, myImage2, xypoints, 40, fupNum);
       for (int pi = 0; pi < 40; pi++)//40个特征点
       {
           //显示patch图
           CVimage fimg = { .width = mybase.patch_d ,.height = mybase.patch_d,.type = CVFGrayType,.data = &mybase.patchModel->datap[pi * mybase.patchModel->col] };
           CVIMAGE myImage1 = YMCV_Creat_Img_Creat(fimg.width, fimg.height, CVGrayType);
           YMCV_Image_Pix_Change_Form(&fimg, myImage1, CV_FGrayToGray2);
           LCD_show_image(xypoints[2 * pi + 0] - mybase.patch_d / 2, xypoints[2 * pi + 1] - mybase.patch_d / 2, myImage1);
           YMCV_Free_ImgFree(myImage1);
       }

       //Cascade人脸检测器
       CV_cascade facedect;
       YMCV_Load_HaarCascade(&facedect, "frontface");//加载 正面(朝前) 人脸检测器
       CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage2, &facedect, 1.0f);//检测人脸

       if (result== NULL) printf("没检测到人脸");
       else if (result->dataNum != 1) printf("检测到多个人脸");
       CVrect* recp = result->datap;

       //和人脸检测器的偏移量训练
       YMCV_FacePoint_Train_bindingFaceDector(&mybase, recp, xypoints, 40, fupNum);

       //绘制人脸框//绘制矩形框
       color24_t rgbval = { .R = 255 };
       LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval);

       YMCV_Free_Vector_U16_Free(result);
       //LCD_FaceFeaturePoint_Show(xypoints, 0, 0);//显示特征点
       Delay(30);
       //PAUSE();
        //释放之前内存
       YMCV_Free_ImgFree(PrImg);
       YMCV_Free_ImgFree(myImage2);
       YMCV_Free_GEOM_RenderFree(geoHrender);
    }

    //测试
    CV_facefpoint_dector facepoint_dect;
    YMCV_Load_FacePointDector(&facepoint_dect, "frontfacepoint");
    for (int i = 0; i < fupNum; i++)//读取30幅图像
    {
        sprintf(myInform, "D:/Documents/1012_YMCV/图片素材/GAN-Face/%d.jpg", i);
        //读取一副图片
        CVIMAGE PrImg = YMCV_Image_Read(myInform, CVGrayType);
        YMCV_Image_Cover(PrImg, Backimg, movex, movey);//合并到底色图片中，这边要和之前读取的时候一致

        LCD_show_image(0, 0, Backimg);

       //Cascade人脸检测器
        CV_cascade facedect;
        YMCV_Load_HaarCascade(&facedect, "frontface");//加载 正面(朝前) 人脸检测器
        CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(Backimg, &facedect, 1.0f);//检测人脸
        CVrect* recp = result->datap;

        CVVECTORS_U16 facepts = YMCV_FacePoint_Detect_Objects(Backimg, &facepoint_dect, recp,15, 20);

        LCD_FaceFeaturePoint_Show(facepts->datap, 0, 0);//显示

        //绘制人脸框//绘制矩形框
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval);
        PAUSE();
        YMCV_Free_Vector_U16_Free(result);
    }
    // ------------------- 显示标准模型和人脸检测框的关系 --------------------------

    CVrect demo = { .x = 200,.y = 100,.w = 150,.h = 150 };
    LCD_Draw_Rect(demo.x, demo.y, demo.w, demo.h, (color24_t){ .R = 255 });
    float32 dcenterx = demo.x + demo.w / 2.0f;
    float32 dcentery = demo.y + demo.h / 2.0f;
    float32 recw = demo.w;
    for (int i = 0; i < 40; i++)
    {
        CVimage fimg = { .width = mybase.patch_d ,.height = mybase.patch_d,.type = CVFGrayType,.data = &mybase.patchModel->datap[i * mybase.patchModel->col] };
        CVIMAGE myImage1 = YMCV_Creat_Img_Creat(fimg.width, fimg.height, CVGrayType);
        YMCV_Image_Pix_Change_Form(&fimg, myImage1, CV_FGrayToGray2);
       
        float px = mybase.standModel->datap[2 * i + 0];
        float py = mybase.standModel->datap[2 * i + 1];

        outdata[2 * i + 0] = px * recw * mybase.facedec_offsets + dcenterx + mybase.facedec_offsetx;
        outdata[2 * i + 1] = py * recw * mybase.facedec_offsets + dcentery + mybase.facedec_offsety;
        //显示图片1
        LCD_show_image(outdata[2 * i + 0]- mybase.patch_d / 2, outdata[2 * i + 1]- mybase.patch_d / 2, myImage1); 
        YMCV_Free_ImgFree(myImage1);
    }
    LCD_FaceFeaturePoint_Show(outdata, 0, 0);//显示

    // ---------------------------- 模型输出 -----------------------
    printf("标准模型：\n");
    for (int i = 0; i < mybase.standModel->dim; i++)//
    {
        printf("%f, ", mybase.standModel->datap[i]);
    }
    printf("\n联合矩阵：\n");
    for (int i = 0; i < mybase.CombBase->row; i++)//
    {
        for (int j = 0; j < mybase.CombBase->col; j++)
        {
            printf("%f, ", mybase.CombBase->datap[i * mybase.CombBase->col +j]);
        }
        printf("\n");
    }
    printf("\n特征描述：\n");
    for (int i = 0; i < mybase.patchModel->row; i++)//
    {
        for (int j = 0; j < mybase.patchModel->col; j++)
        {
            printf("%f, ", mybase.patchModel->datap[i * mybase.patchModel->col + j]);
        }
        printf("\n");
    }
    printf("\n d:%d\n", mybase.patch_d);
    printf("\n offset: %f,%f,%f\n", mybase.facedec_offsetx, mybase.facedec_offsety, mybase.facedec_offsets);
    //释放内存
    YMCV_Free_ImgFree(calibrat);
    YMCV_Free_ImgFree(Backimg);

    PAUSE();
    LCD_Destory();
    return 0;
}


////                          0                          9   10                19  20    22 23    25 26 27 28 29 30 31 32 33 34 35,36,37 38 39
//int8 ReflectiveIndex[40] = { 10,11,12,13,14,15,16,17,18,19, 0,1,2,3,4,5,6,7,8,9, 25,24,23,22,21,20,29,28,27,26,31,30,33,32,35,34,37,36,38,39 };
////左右对称的样本点，进行样本数增加
//for (int dai = 0; dai < dataNum; dai++)
//{
//    CVfpoint* xout0 = &Xredo->datap[dai * Xredo->dim];
//    CVfpoint* xout1 = &Xredo->datap[(dataNum + dai) * Xredo->dim];//第二组
//    //数据x轴反对称
//    for (int i = 0; i < 40; i++)
//    {
//        int rdi = ReflectiveIndex[i];
//        xout1[i].x = -xout0[rdi].x;
//        xout1[i].y = xout0[rdi].y;
//
//        color24_t pencolor = YMCV_HeatPalette[((i + 1) * 389) % 255];
//        //绘制特征点
//        LCD_Fill_ConstRgbRect(200 + xout0[i].x - 1, 200 + xout0[i].y - 1, 3, 3, pencolor);
//        LCD_Fill_ConstRgbRect(200 + xout1[i].x - 1, 200 + xout1[i].y - 1, 3, 3, pencolor);
//    }
//}

