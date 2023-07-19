#ifndef _YMCV_OBJ_H
#define _YMCV_OBJ_H
#include"../CONFIG/YMCV_PubType.h"

//图像
typedef enum
{
	CVUnDefineType = 0,
	CVBinaryType = 1,// 二值图
	CVTrimapType,//三色图
	CVGrayType,//灰度图
	CVRgb16Type,//RGB565
	CVFGrayType,//float gray
	CVIGrayType,//int gray
	CVCGrayType,//complex gray
}CVImgType;

//图像
typedef struct 
{
	uint16 width;
	uint16 height;
	uint8 pmlc; //图像完成标识
	size_t sizet;
	CVImgType type;
	_color8_t* data;
}CVimage;
typedef CVimage* CVIMAGE;

//坐标点
typedef struct
{
	uint16 x, y;
}CVpoint;
//矩形
typedef struct
{
	uint16 x, y, w, h;
}CVrect;
//线段
typedef struct
{
	int16 x1;
	int16 y1;
	int16 x2;
	int16 y2;
}CVline;

//点
typedef struct
{
	float x;
	float y;
}CVfpoint;

//线段
typedef struct
{
	CVfpoint pa;
	CVfpoint pb;
}CVfline;

//三角形
typedef struct
{
	CVfpoint A, B, C;
}CVftriangle;

//向量
typedef struct
{
	float32* datap;
	uint32 dim : 10;//10bit  max=1024
	uint32 dataNum : 22;//22bit max=4194304 = 419*10000
}CVvectors_f32;
typedef CVvectors_f32* CVVECTORS_F32;

typedef struct
{
	uint16* datap;
	uint32 dim : 8;//8bit  max=255
	uint32 dataNum : 24;//24bit max=16777216 = 1677*10000
}CVvectors_u16;
typedef CVvectors_u16* CVVECTORS_U16;

//矩阵
typedef struct
{
	float32* datap;
	uint16 row;
	uint16 col;
}CVfmat;
typedef CVfmat* CVFMAT;

//链码
typedef struct
{
	CVVECTORS_U16* chain;
	uint32 cNum;//链码数
}CVchains_u16;
typedef CVchains_u16* CVCHAINS_U16;

//人脸LBP等价模式直方图
typedef struct
{
	uint8 hist[7 * 7 * 59];// 区域为 7×7大小 ,等价模式59种
} CV_face_lbp_desc;
typedef CV_face_lbp_desc* CV_FACE_LBP_DESC;

//目标跟踪器
typedef struct
{
	CVrect roi;
	uint16 histnum;
	float32* hist;
	float32* Kdis;
}CV_target_track;
typedef CV_target_track* CV_TARGET_TRACK;

 //粒子状态空间变量 
typedef struct { 
	int x;//x坐标位置 
	int y;//x坐标位置 
	int Hx;//x方向半窗宽 
	int Hy;//y方向半窗宽 
	float vx;//x方向运动速度 
	float vy;//y方向运动速度 
	float vs;//尺度变换速度 
} CV_particle_state;

//粒子跟踪器
typedef struct
{
	uint8 particleNum;//粒子个数
	uint16 histNum;//直方图个数
	CVrect result;
	float32 velocity_delta;//速度扰动幅值
	float32 scale_delta;//尺度变换速度扰动幅度
	CV_particle_state* states;
	float32* weights;
	float32* hists;
}CV_particle_track;
typedef CV_particle_track* CV_PARTICLE_TRACK;

#endif

