#ifndef YMCV_MATCHINGCAL_H
#define YMCV_MATCHINGCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

typedef enum
{
	CV_Template_MAD,//平均绝对差算法
	CV_Template_SSDA,//序贯相似性检测算法
	CV_Template_NCC,//归一化积相关算法
}CV_TemplateType;

float32 YMCV_Image_Gray_SSIM_Cal(CVIMAGE myimg1, CVIMAGE myimg2);//结构相似性计算
CVrect YMCV_Gray_Matching_Template(CVIMAGE temp, CVIMAGE myimg1, CV_TemplateType type, uint8 skipStep, uint8 error_max);//模板匹配
void YMCV_Image_Gray_PhaseCorrelate(CVIMAGE imageTemp, CVIMAGE myimg2, float32* moveX, float32* moveY);//相位相关匹配
void YMCV_FeatureDescribe_Match(CVVECTORS_U16 tempFeatDesc, CVVECTORS_U16 myFeatDesc, uint16* resultID, uint8* matched, uint8 matchThreshold);//特征描述符匹配
void YMCV_Gray_MeanShift_Match(CV_TARGET_TRACK track, CVIMAGE image, uint16 maxIter);//MeanShift匹配（跟踪）
uint8 YMCV_GrayParticleTracking(CV_PARTICLE_TRACK track, CVIMAGE image);//粒子滤波跟踪

/* Haar cascade 分类器*/
typedef struct CV_cascade_
{
	int32 std;                      //图像的标准差。
	uint32* sum;                    //积分图像。
	uint32* ssq;                    //平方积分图像。

	int32 stages_num;               //cascade中的stage数。
	int32 weak_num;                 //cascade中的弱分类器(特征)数量。
	int32 rects_num;                //级联（cascade）中矩形的数目。

	int32 window_w;					//检测窗口大小。
	int32 window_h;

	uint8* perstage_weak_num;  //每个stage的弱分类器数量。
	int16* perstage_thresh;    //stage的阈值。
	int16* perweak_thresh;     // 每个弱分类器的阈值。
	int16* alpha_neg;          // 每个弱分类区的权重（分错）。
	int16* alpha_pos;          // 每个弱分类区的权重（分对）。
	int8* perweak_rects_num;   // 每个弱分类器对应的矩形数。
	int8* perrects_weights;    // 每个矩形的对应的权重。
	int8* perrects_xywh;       // 矩形数组坐标。
} CV_cascade;
void YMCV_Load_HaarCascade(CV_cascade* cascade, const char* detctype);//加载检测器
CVVECTORS_U16 YMCV_HaarCascade_Detect_Objects(CVIMAGE myimg, CV_cascade* cascade, float  reserved_rate);//对象检测

float32 YMCV_Face_LBP_Hist_X2_Distance(CV_FACE_LBP_DESC lbp0, CV_FACE_LBP_DESC lbp1);//卡方检验

//人脸特征点检测器
typedef struct
{
	float32* standModel;//标准模型
	float32* combBases;//数据的联合分布矩阵
	float32* point_Patch;//特征点描述patch
	int32 facepoint_num;//特征点个数
	uint8 patch_d;//特征描述patch半径
	uint8 combmat_col;//联合矩阵列数
	float32 scale;//特征缩放尺寸
	float32 facedec_offsetx;//和人脸检测器的坐标偏移
	float32 facedec_offsety;
	float32 facedec_offsets;//缩放
}
CV_facefpoint_dector;
void YMCV_Load_FacePointDector(CV_facefpoint_dector* dector, const char* detctype);//加载检测器
CVVECTORS_U16 YMCV_FacePoint_Detect_Objects(CVIMAGE myimg, CV_facefpoint_dector* dector, CVrect* facerect, uint8 search_r, int maxIter);//人脸特征点检测

#endif

