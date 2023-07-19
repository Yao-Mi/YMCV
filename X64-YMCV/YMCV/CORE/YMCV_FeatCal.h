#ifndef YMCV_FEATCAL_H
#define YMCV_FEATCAL_H
#include"../OPOBJ/YMCV_OBJ.h"
#include"./YMCV_BinaryCal.h"

//blob��С������2�ֽڵı���
typedef struct
{
	CVrect box; //4*2�ֽ�
	CVpoint seed;//2*2�ֽ�

	//2*2�ֽ�
	uint32 label : 8;//8bit  max=255
	uint32 pixelsNum : 24;//24bit max=16777216 = 1677*10000
}CVblob;

typedef struct
{
	CVThreshold ThreshType;//��ֵ������
	CV_binary_vec myVector;//��������
	uint8 threshnum;
}CVblob_seg_render;
typedef CVblob_seg_render* CVBLOB_SEG_RENDER;


CVVECTORS_U16 YMCV_Image_ColorBox_Find(CVIMAGE myimg, CVcolor minVal, CVcolor maxVal, uint32 minArea, uint8 showSeg);//ɫ�����
void YMCV_BlobSegKernel_Load(CVBLOB_SEG_RENDER render, CV_binary_param* Param, uint8 paramArrayNum);//ɫ��ָ��������
CVVECTORS_U16 YMCV_Image_Find_ColorBlobs(CVIMAGE myimg, CVBLOB_SEG_RENDER blob_render, uint32 minArea, uint8 showSegX);//ɫ����� ����ָ�ͬʱ����
CVVECTORS_U16 YMCV_WormFindLines(CVIMAGE edgebinary, float32 dir_update_k, int minLen,uint8 useStride);//ֱ�߼��
void YMCV_Image_Binary_Bone_Thin(CVIMAGE myimg);//��ֵͼ�Ǽ�ϸ��
void YMCV_Image_Binary_Bone_Cut(CVIMAGE myimg, uint16 LenThreshold);//�Ǽܼ�֦
CVCHAINS_U16 YMCV_Image_Binary_Edge_Trace(CVIMAGE myimg, CVVECTORS_U16 blobs);//��ֵͼ�߽����

//lbp����ͼ����
typedef enum
{
	CV_FeatMap_LBP,//��ͳLBP
	CV_FeatMap_MRLBP,//��Сѭ��LBP
	CV_FeatMap_USAN,//��ֵ������
}CVFeatureMapType;
CVIMAGE YMCV_Image_FeatureMap_Detect(CVIMAGE myimg, CVFeatureMapType type);//����ͼ���

//����������
typedef enum
{
	CV_FeatPoint_SUSAN,//
	CV_FeatPoint_Fast,// Fast- 9 - 16 ��susan�ĸĽ��㷨
	CV_FeatPoint_AGAST,// ����
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
CVVECTORS_U16 YMKV_FeaturePoint_Find(CVIMAGE myimg, CV_featpoint_param* param);//����ͼ���


typedef struct
{
	int16 angle;
	uint8 desc[32];//32byte������
} CV_featdesc_type;
CVVECTORS_U16 YMCV_FeatureDescribe_RotateBrief_Cal(CVIMAGE myimg, CVVECTORS_U16 featpoints);//brief����������ȡ������ת��ȡ���ķ���Ϊ������

//hog����
typedef struct
{
	float32 hist[9];// 9���Ƕ�[0,20,40,60,80,120,140,160]
}CVhog_cell8x8;
typedef CVhog_cell8x8* CVHOG_CELL8X8;
CVVECTORS_F32 YMCV_Gray_Find_Hog_Describe(CVIMAGE myimg);//hog��������ȡ

//����lbpֱ��ͼ��������ȡ
CV_FACE_LBP_DESC YMCV_Face_UniformLBP_Describe(CVIMAGE faceImage);//lbp������

//���Իع�
typedef enum
{
	CVRegress_LSE,// LSE(least square estimator )//��С���˹���
	CVRegress_TSE,// TSE(Theil-Sen estimator) //̩��-ɭ ����
	CVRegress_RANSAC,// Ransac����
}CVregress_type;
CVline YMCV_BinaryMax25x25_Regression_Line(CVIMAGE myimg, CVregress_type Regress_Type);//��ֵͼ���ػع�

CVVECTORS_U16 YMCV_Binary_XY_Axis_Proj(CVIMAGE myimg, uint8 axis_0x_1y);//��ֵͼx,y��ͶӰ
CVIMAGE YMCV_Binary_MaxPooling_ScaleSizeTo_MxM(CVIMAGE myimg, uint8 sizeM);//����ͼ���СΪM��M���������ֵ�ػ�������
CVVECTORS_U16 YMCV_Binary_Number16x16_ProjHist16(CVIMAGE myimg);//16x16��ͼ��������ȡ
CVIMAGE YMCV_Binary_To_Hough(CVIMAGE myimg, uint8 theta_shift_step);//����ռ�
CVIMAGE YMCV_Gray_Radon_Convert(CVIMAGE myimg, uint8 theta_shift_step, uint8 usehough);//�����任
CVIMAGE YMCV_Gray_Radon_IConvert(CVIMAGE myimg, uint8 theta_shift_step);//�������任
CVIMAGE YMCV_Gray_Stereo_LineMatching(CVIMAGE left, CVIMAGE right, uint8 searchWidth, uint8 mergeThresh);//����ƥ��
// ED ϵ��

//�ݶ�����
typedef enum
{
	CV_Prewitt_Graident,//Prewitt
	CV_Sobel_Graident,//Sobel
	CV_Scharr_Graident,//Scharr
}CVGraidentType;

//��Ե��ȡ
typedef struct CV_ed_point_
{
	CVpoint pos;//λ�ã�������ڿ�ͷ
	float32 ang;//�ݶȷ���
}CV_ed_point;
CVCHAINS_U16 YMCV_Fast_EdgeDrawing(CVIMAGE gray, CVGraidentType gtype, uint16 gradMinLimit, CVIMAGE edge);//��Ե����

//ֱ�߼��
typedef struct CV_ed_line_ {
	CVline start_end;
	//��׼��ʽ���� Ax+By+C=0
	float LineA, LineB, LineC;
}CV_ed_line;
CVVECTORS_U16 YMCV_Fast_EdgeDrawing_Lines(CVCHAINS_U16 chains, float line_error, int min_line_len);//ֱ�߲���

//��Բ���
typedef struct CV_ed_ellipse_ {
	CVpoint center;
	float aAxis, bAxis,aAngle;//a,b��ͳ������
	//��׼��ʽ���� Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
	float eA, eB, eC, eD, eF;
}CV_ed_ellipse;

//͹ת�������
typedef struct CV_ed_corner_
{
	CVpoint center;//����λ��
	uint16 max_r; //Զ��Բ�뾶
	uint16 min_r; //����Բ�뾶
	//     ______
	//    / \ /  \
	//   |---*----| ��
	//    \ / \  /
	//     \____/
	//      ��
	uint16 maxLenAngle;//��Զ����ĽǶȣ�˳ʱ�룩
	uint16 CornerNum;//ת�Ǵ���
}CV_ed_corner;
CVVECTORS_U16 YMCV_Fast_EdgeDrawing_ChainsCornerNum(CVCHAINS_U16 chains, int skipShorterNum);//�պ����� ������⣬������͹����μ��


//������������ѵ��
typedef struct
{
	CVVECTORS_F32 standModel;//��׼ģ��
	CVFMAT alignMat;//�������Ĳ�������ת����+��������
	CVFMAT CombBase;//���ݵ����Ϸֲ�����
	CVFMAT Eu;
	CVFMAT patchModel;//����������
	uint8 patch_d;
	float32 scale;//�������ųߴ�
	float32 facedec_offsetx;//�����������������ƫ��
	float32 facedec_offsety;
	float32 facedec_offsets;//����
}CV_facefpoint_train;

CV_facefpoint_train YMCV_FacePoint_Train_Bases(CVVECTORS_F32 Xpts);//�������ֲ�������
void YMCV_FacePoint_Train_Patch(CV_facefpoint_train* trans, CVIMAGE grayimg, uint16 xypoints[], uint16 pointsNum, uint16 totalTranTimes);//������patch������ȡ
void YMCV_FacePoint_Train_bindingFaceDector(CV_facefpoint_train* trans, CVrect* faceRect, uint16 xypoints[], uint16 pointsNum, uint16 totalTranTimes);//�����������


#endif // !YMCV_FEATCAL_H

