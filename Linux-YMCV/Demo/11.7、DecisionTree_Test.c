#include "../SDL_LCD/SDL_LCD.h"
#include"../YMCV/CORE/YMCV_PubData.h"
#include"../YMCV/CORE/YMCV_DecisionTree_ID3.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CORE/YMCV_InnerMath.h"

int main()
{
    LCD_Init(1000, 450);

    /*-------------------------- ID3决策树带标签数据训练和分类测试 --------------------------------------------------*/
    CVIMAGE myImagek = YMCV_Creat_Img_Creat(250, 150, CVGrayType);
    LCD_show_image(0, 0, myImagek);//绘图背景刷新

    YMCV_Srand(99);//初始化随机数种子

#define dataN 500 //数据量2000个

    //数据标签，需要和待分类数据个数相同
    uint8 labels[dataN] = { 1,0,1,2,2,1,1,3,0,3,3,0,3,3,2,1,4,0,2,1,3,3,0,3,1,4,3,3,3,3,1,1,4,0,0,4,0,3,2,2,3,1,3,4,3,3,3,1,4,2,2,0,0,0,1,3,4,0,1,2,0,4,4,1,4,3,4,0,1,1,1,3,3,4,1,0,0,0,0,0,1,0,2,1,4,1,1,2,2,1,1,0,2,2,0,3,0,3,0,1,0,3,1,2,2,0,0,1,0,1,1,0,1,2,2,4,2,2,3,2,2,1,2,4,2,2,2,1,3,0,3,4,3,2,2,2,3,4,3,3,2,3,0,1,3,0,3,0,3,3,4,3,1,0,3,2,3,1,4,0,4,0,0,2,4,1,3,2,0,4,2,1,1,0,1,3,3,4,0,1,1,4,2,0,2,4,4,3,1,3,0,2,3,2,3,3,3,3,4,2,4,4,1,4,1,0,1,2,0,0,1,4,1,4,2,1,2,4,0,3,4,1,0,3,3,3,1,1,3,0,0,2,0,0,1,0,3,4,0,0,1,1,3,0,3,1,2,2,0,2,2,3,1,3,2,1,1,1,4,4,0,4,4,1,0,2,1,1,1,3,4,0,0,4,3,3,4,1,3,2,1,1,2,1,3,1,1,0,3,3,0,0,2,0,3,4,0,1,0,2,0,1,0,2,4,3,1,2,3,3,1,3,3,1,2,2,3,1,4,2,4,4,1,3,1,2,1,2,3,1,0,2,3,2,3,4,4,3,3,4,1,2,4,2,1,1,4,2,2,3,3,3,3,4,4,2,3,2,3,3,2,0,2,0,1,3,2,3,1,0,4,3,2,3,3,0,2,3,3,4,1,2,4,3,3,4,3,3,1,2,1,1,4,0,3,0,1,3,3,3,1,2,2,1,0,3,1,3,0,2,3,3,0,2,4,3,4,3,1,0,0,1,4,4,2,2,0,3,4,2,1,3,1,3,3,4,4,4,4,1,2,0,0,4,1,0,4,4,4,0,0,1,3,4,3,1,3,1,0,3,3,4,4,0,4,2,0,3,0,4,1,4,4,1,3,3,1,4,3,0,0,3,3,1,2,3,0,1,2,1,0,4,4,4,4,2,3,3,4,0, };

    CVVECTORS_U16 myvec = YMCV_Creat_Vector_U16(2, dataN);//创建向量

    for (int i = 0; i < myvec->dataNum; i++)//初始化向量值
    {
        int16* thisvec = (int16*)myvec->datap + i * myvec->dim;
        thisvec[0] = YMCV_Randint() % 250;//宽
        thisvec[1] = YMCV_Randint() % 150;//高
    }

    uint8 Knum = 5;

    // 采用kmeans预先分类 得到决策树训练数据
    //CVVECTORS_F32 center = YMCV_Creat_Vector_F32(2, Knum);//创建k个中心,向量维度与数据相同
    ////Kmeans数据中心提取
    //YMCV_FVector_Kmeans_GetCenter(myvec, NULL, center, 100);//labels标签重新分类
    //YMCV_FVector_Kmeans_Classify(myvec, center, labels);


    //ID3决策树创建
    CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//创建决策模型

    YMCV_DecisionTree_ID3_Trains(myID3, myvec, labels);//ID3决策树数据训练

    //决策树数据分类
    YMCV_DecisionTree_Classify(myvec, labels,myID3);

    //绘制数据
    for (int i = 0; i < myvec->dataNum; i++)
    {
        int16* thisvec = (int16*)myvec->datap + i * myvec->dim;
        int cl = 255 * labels[i] / (Knum - 1);//类别转颜色索引
        //printf("%d,", labels[i]);
        //绘制数据小矩形，颜色从热力表中拿
        LCD_Fill_ConstRgbRect(thisvec[0] - 1, thisvec[1] - 1, 2, 2, YMCV_HeatPalette[cl]);
    }

    PAUSE();
    YMCV_DecisionTree_Free(myID3);//释放模型
    YMCV_Free_ImgFree(myImagek);
}