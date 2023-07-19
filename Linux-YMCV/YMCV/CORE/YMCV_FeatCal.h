#ifndef YMCV_FEATCAL_H
#define YMCV_FEATCAL_H
#include"../OPOBJ/YMCV_OBJ.h"
#include"./YMCV_BinaryCal.h"

//blob大小必须是2字节的倍数
typedef struct
{
	CVrect box; //4*2字节
	CVpoint seed;//2*2字节

	//2*2字节
	uint32 label : 8;//8bit  max=255
	uint32 pixelsNum : 24;//24bit max=16777216 = 1677*10000
}CVblob;

typedef struct
{
	CVThreshold ThreshType;//二值化类型
	CV_binary_vec myVector;//缓存向量
	uint8 threshnum;
}CVblob_seg_render;
typedef CVblob_seg_render* CVBLOB_SEG_RENDER;


CVVECTORS_U16 YMCV_Image_ColorBox_Find(CVIMAGE myimg, CVcolor minVal, CVcolor maxVal, uint32 minArea, uint8 showSeg);//色块查找
void YMCV_BlobSegKernel_Load(CVBLOB_SEG_RENDER render, CV_binary_param* Param, uint8 paramArrayNum);//色块分割参数加载
CVVECTORS_U16 YMCV_Image_Find_ColorBlobs(CVIMAGE myimg, CVBLOB_SEG_RENDER blob_render, uint32 minArea, uint8 showSegX);//色块查找 多组分割同时进行
CVVECTORS_U16 YMCV_WormFindLines(CVIMAGE edgebinary, float32 dir_update_k, int minLen,uint8 useStride);//直线检测
void YMCV_Image_Binary_Bone_Thin(CVIMAGE myimg);//二值图骨架细化
void YMCV_Image_Binary_Bone_Cut(CVIMAGE myimg, uint16 LenThreshold);//骨架剪枝
CVCHAINS_U16 YMCV_Image_Binary_Edge_Trace(CVIMAGE myimg, CVVECTORS_U16 blobs);//二值图边界跟踪

//lbp特征图类型
typedef enum
{
	CV_FeatMap_LBP,//传统LBP
	CV_FeatMap_MRLBP,//最小循环LBP
	CV_FeatMap_USAN,//核值相似区
}CVFeatureMapType;
CVIMAGE YMCV_Image_FeatureMap_Detect(CVIMAGE myimg, CVFeatureMapType type);//特征图检测

//特征点类型
typedef enum
{
	CV_FeatPoint_SUSAN,//
	CV_FeatPoint_Fast,// Fast- 9 - 16 是susan的改进算法
	CV_FeatPoint_AGAST,// 最快的
	CV_FeatPoint_Harris,//Harris
}CVFeatPointpType;
typedef  struct
{
	CVFeatPointpType type;
	union 
	{
		//SUSAN
		struct
		{
			uint16 thresh;
		}susan;
		//fast
		struct
		{
			int8 thresh;
		}fast;
		//AGAST
		struct
		{
			int8 thresh;
		}agast;
		//Harris
		struct
		{
			uint8 thresh;
		}harris;
	}param;
}CV_featpoint_param;
CVVECTORS_U16 YMKV_FeaturePoint_Find(CVIMAGE myimg, CV_featpoint_param* param);//特征图检测


typedef struct
{
	int16 angle;
	uint8 desc[32];//32byte描述符
} CV_featdesc_type;
CVVECTORS_U16 YMCV_FeatureDescribe_RotateBrief_Cal(CVIMAGE myimg, CVVECTORS_U16 featpoints);//brief特征描述提取，带旋转，取重心方向为正方向

//hog特征
typedef struct
{
	float32 hist[9];// 9个角度[0,20,40,60,80,120,140,160]
}CVhog_cell8x8;
typedef CVhog_cell8x8* CVHOG_CELL8X8;
CVVECTORS_F32 YMCV_Gray_Find_Hog_Describe(CVIMAGE myimg);//hog描述符提取

//人脸lbp直方图描述符提取
CV_FACE_LBP_DESC YMCV_Face_UniformLBP_Describe(CVIMAGE faceImage);//lbp描述符

//线性回归
typedef enum
{
	CVRegress_LSE,// LSE(least square estimator )//最小二乘估计
	CVRegress_TSE,// TSE(Theil-Sen estimator) //泰尔-森 估计
	CVRegress_RANSAC,// Ransac估计
}CVregress_type;
CVline YMCV_BinaryMax25x25_Regression_Line(CVIMAGE myimg, CVregress_type Regress_Type);//二值图像素回归

CVVECTORS_U16 YMCV_Binary_XY_Axis_Proj(CVIMAGE myimg, uint8 axis_0x_1y);//二值图x,y轴投影
CVIMAGE YMCV_Binary_MaxPooling_ScaleSizeTo_MxM(CVIMAGE myimg, uint8 sizeM);//缩放图像大小为M×M，采用最大值池化法缩放
CVVECTORS_U16 YMCV_Binary_Number16x16_ProjHist16(CVIMAGE myimg);//16x16的图像特征提取
CVIMAGE YMCV_Binary_To_Hough(CVIMAGE myimg, uint8 theta_shift_step);//霍夫空间
CVIMAGE YMCV_Gray_Radon_Convert(CVIMAGE myimg, uint8 theta_shift_step, uint8 usehough);//拉东变换
CVIMAGE YMCV_Gray_Radon_IConvert(CVIMAGE myimg, uint8 theta_shift_step);//拉东反变换
CVIMAGE YMCV_Gray_Stereo_LineMatching(CVIMAGE left, CVIMAGE right, uint8 searchWidth, uint8 mergeThresh);//立体匹配
// ED 系列

//梯度类型
typedef enum
{
	CV_Prewitt_Graident,//Prewitt
	CV_Sobel_Graident,//Sobel
	CV_Scharr_Graident,//Scharr
}CVGraidentType;

//边缘提取
typedef struct CV_ed_point_
{
	CVpoint pos;//位置，必须放在开头
	float32 ang;//梯度方向
}CV_ed_point;
CVCHAINS_U16 YMCV_Fast_EdgeDrawing(CVIMAGE gray, CVGraidentType gtype, uint16 gradMinLimit, CVIMAGE edge);//边缘绘制

//直线检测
typedef struct CV_ed_line_ {
	CVline start_end;
	//标准形式参数 Ax+By+C=0
	float LineA, LineB, LineC;
}CV_ed_line;
CVVECTORS_U16 YMCV_Fast_EdgeDrawing_Lines(CVCHAINS_U16 chains, float line_error, int min_line_len);//直线查找

//椭圆检测
typedef struct CV_ed_ellipse_ {
	CVpoint center;
	float aAxis, bAxis,aAngle;//a,b轴和长轴倾角
	//标准形式参数 Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
	float eA, eB, eC, eD, eF;
}CV_ed_ellipse;

//凸转角数检测
typedef struct CV_ed_corner_
{
	CVpoint center;//中心位置
	uint16 max_r; //远接圆半径
	uint16 min_r; //近接圆半径
	//     ______
	//    / \ /  \
	//   |---*----| ↓
	//    \ / \  /
	//     \____/
	//      ←
	uint16 maxLenAngle;//最远距离的角度（顺时针）
	uint16 CornerNum;//转角次数
}CV_ed_corner;
CVVECTORS_U16 YMCV_Fast_EdgeDrawing_ChainsCornerNum(CVCHAINS_U16 chains, int skipShorterNum);//闭合链码 角数检测，可用于凸多边形检测


//人脸特征点检测训练
typedef struct
{
	CVVECTORS_F32 standModel;//标准模型
	CVFMAT alignMat;//对齐矩阵的参数，旋转矩阵+中心坐标
	CVFMAT CombBase;//数据的联合分布矩阵
	CVFMAT Eu;
	CVFMAT patchModel;//特征点描述
	uint8 patch_d;
	float32 scale;//特征缩放尺寸
	float32 facedec_offsetx;//和人脸检测器的坐标偏移
	float32 facedec_offsety;
	float32 facedec_offsets;//缩放
}CV_facefpoint_train;

CV_facefpoint_train YMCV_FacePoint_Train_Bases(CVVECTORS_F32 Xpts);//求特征分布基向量
void YMCV_FacePoint_Train_Patch(CV_facefpoint_train* trans, CVIMAGE grayimg, uint16 xypoints[], uint16 pointsNum, uint16 totalTranTimes);//特征点patch描述提取
void YMCV_FacePoint_Train_bindingFaceDector(CV_facefpoint_train* trans, CVrect* faceRect, uint16 xypoints[], uint16 pointsNum, uint16 totalTranTimes);//绑定人脸检测器


#endif // !YMCV_FEATCAL_H

