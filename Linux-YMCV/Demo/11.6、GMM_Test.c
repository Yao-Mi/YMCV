#include "../SDL_LCD/SDL_LCD.h"
#include"../YMCV/CORE/YMCV_PubData.h"
#include"../YMCV/CORE/YMCV_GMM.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CORE/YMCV_InnerMath.h"

int main()
{
    LCD_Init(1000, 450);

    /*-------------------------- gmms带标签数据训练和分类测试 --------------------------------------------------*/
    CVIMAGE myImagek = YMCV_Creat_Img_Creat(250, 150, CVGrayType);
    LCD_show_image(0, 0, myImagek);//绘图背景刷新

    YMCV_Srand(99);//初始化随机数种子

#define dataN 500 //数据量2000个

    //数据标签，需要和待分类数据个数相同
    uint8 labels[dataN] = { 1,0,1,2,2,1,1,3,0,3,3,0,3,3,2,1,4,0,2,1,3,3,0,3,1,4,3,3,3,3,1,1,4,0,0,4,0,3,2,2,3,1,3,4,3,3,3,1,4,2,2,0,0,0,1,3,4,0,1,2,0,4,4,1,4,3,4,0,1,1,1,3,3,4,1,0,0,0,0,0,1,0,2,1,4,1,1,2,2,1,1,0,2,2,0,3,0,3,0,1,0,3,1,2,2,0,0,1,0,1,1,0,1,2,2,4,2,2,3,2,2,1,2,4,2,2,2,1,3,0,3,4,3,2,2,2,3,4,3,3,2,3,0,1,3,0,3,0,3,3,4,3,1,0,3,2,3,1,4,0,4,0,0,2,4,1,3,2,0,4,2,1,1,0,1,3,3,4,0,1,1,4,2,0,2,4,4,3,1,3,0,2,3,2,3,3,3,3,4,2,4,4,1,4,1,0,1,2,0,0,1,4,1,4,2,1,2,4,0,3,4,1,0,3,3,3,1,1,3,0,0,2,0,0,1,0,3,4,0,0,1,1,3,0,3,1,2,2,0,2,2,3,1,3,2,1,1,1,4,4,0,4,4,1,0,2,1,1,1,3,4,0,0,4,3,3,4,1,3,2,1,1,2,1,3,1,1,0,3,3,0,0,2,0,3,4,0,1,0,2,0,1,0,2,4,3,1,2,3,3,1,3,3,1,2,2,3,1,4,2,4,4,1,3,1,2,1,2,3,1,0,2,3,2,3,4,4,3,3,4,1,2,4,2,1,1,4,2,2,3,3,3,3,4,4,2,3,2,3,3,2,0,2,0,1,3,2,3,1,0,4,3,2,3,3,0,2,3,3,4,1,2,4,3,3,4,3,3,1,2,1,1,4,0,3,0,1,3,3,3,1,2,2,1,0,3,1,3,0,2,3,3,0,2,4,3,4,3,1,0,0,1,4,4,2,2,0,3,4,2,1,3,1,3,3,4,4,4,4,1,2,0,0,4,1,0,4,4,4,0,0,1,3,4,3,1,3,1,0,3,3,4,4,0,4,2,0,3,0,4,1,4,4,1,3,3,1,4,3,0,0,3,3,1,2,3,0,1,2,1,0,4,4,4,4,2,3,3,4,0, };

    CVVECTORS_F32 myvec = YMCV_Creat_Vector_F32(2, dataN);//创建向量

    for (int i = 0; i < myvec->dataNum; i++)//初始化向量值
    {
        float32* thisvec = myvec->datap + i * myvec->dim;
        thisvec[0] = YMCV_Randint() % 250;//宽
        thisvec[1] = YMCV_Randint() % 150;//高
    }

    uint8 Knum = 5;//k=5个分布

    //CVVECTORS_F32 center = YMCV_Creat_Vector_F32(2, Knum);//创建k个中心,向量维度与数据相同
    ////Kmeans数据中心提取
    //YMCV_FVector_Kmeans_GetCenter(myvec, NULL, center, 100);//labels标签重新分类
    //YMCV_FVector_Kmeans_Classify(myvec, center, labels);
    //for (int i = 0; i < dataN; i++)
    //{
    //    printf("%d,", labels[i]);
    //}
    
    //GMMS训练
    CVGMMS myGMM = YMCV_Creat_Gmms_Creat(myvec->dim, Knum);//创建高斯混合模型
    YMCV_Fdata_GMMs_Learning(myGMM, myvec, labels);//数据训练

    //GMMS数据分类
    for (int i = 0; i < myvec->dataNum; i++)
    {
        float32* thisvec = myvec->datap + i * myvec->dim;
        labels[i] = YMCV_Fdata_GMMs_belongWhichComponent(myGMM, thisvec, myvec->dim);//数据分类
    }

    printf("det:");
    for (int i = 0; i < myGMM->componentsNum; i++)
        printf("%f ", myGMM->covdet[i]);
    //绘制数据
    for (int i = 0; i < myvec->dataNum; i++)
    {
        float32* thisvec = myvec->datap + i * myvec->dim;
        int cl = 255 * labels[i] / (Knum - 1);//类别转颜色索引
        //printf("%d,", labels[i]);
        //绘制数据小矩形，颜色从热力表中拿
        LCD_Fill_ConstRgbRect(thisvec[0] - 1, thisvec[1] - 1, 2, 2, YMCV_HeatPalette[cl]);
    }
    //绘制高斯分布的中心点
    for (int i = 0; i < Knum; i++)
    {
        float32* thisvec = myGMM->mean + i * myGMM->dataDim;
        int cl = 255 * i / (Knum - 1);//类别转颜色索引

        //绘制数据大矩形，颜色从热力表中拿
        LCD_Fill_ConstRgbRect(thisvec[0] - 2, thisvec[1] - 2, 5, 5, YMCV_HeatPalette[cl]);
    }

    PAUSE();
    YMCV_Free_Gmms_Free(myGMM);
    YMCV_Free_ImgFree(myImagek);
}