#ifndef YMCV_MATCHINGCAL_H
#define YMCV_MATCHINGCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

typedef enum
{
	CV_Template_MAD,//ƽ�����Բ��㷨
	CV_Template_SSDA,//��������Լ���㷨
	CV_Template_NCC,//��һ��������㷨
}CV_TemplateType;

float32 YMCV_Image_Gray_SSIM_Cal(CVIMAGE myimg1, CVIMAGE myimg2);//�ṹ�����Լ���
CVrect YMCV_Gray_Matching_Template(CVIMAGE temp, CVIMAGE myimg1, CV_TemplateType type, uint8 skipStep, uint8 error_max);//ģ��ƥ��
void YMCV_Image_Gray_PhaseCorrelate(CVIMAGE imageTemp, CVIMAGE myimg2, float32* moveX, float32* moveY);//��λ���ƥ��
void YMCV_FeatureDescribe_Match(CVVECTORS_U16 tempFeatDesc, CVVECTORS_U16 myFeatDesc, uint16* resultID, uint8* matched, uint8 matchThreshold);//����������ƥ��
void YMCV_Gray_MeanShift_Match(CV_TARGET_TRACK track, CVIMAGE image, uint16 maxIter);//MeanShiftƥ�䣨���٣�
uint8 YMCV_GrayParticleTracking(CV_PARTICLE_TRACK track, CVIMAGE image);//�����˲�����

/* Haar cascade ������*/
typedef struct CV_cascade_
{
	int32 std;                      //ͼ��ı�׼�
	uint32* sum;                    //����ͼ��
	uint32* ssq;                    //ƽ������ͼ��

	int32 stages_num;               //cascade�е�stage����
	int32 weak_num;                 //cascade�е���������(����)������
	int32 rects_num;                //������cascade���о��ε���Ŀ��

	int32 window_w;					//��ⴰ�ڴ�С��
	int32 window_h;

	uint8* perstage_weak_num;  //ÿ��stage����������������
	int16* perstage_thresh;    //stage����ֵ��
	int16* perweak_thresh;     // ÿ��������������ֵ��
	int16* alpha_neg;          // ÿ������������Ȩ�أ��ִ���
	int16* alpha_pos;          // ÿ������������Ȩ�أ��ֶԣ���
	int8* perweak_rects_num;   // ÿ������������Ӧ�ľ�������
	int8* perrects_weights;    // ÿ�����εĶ�Ӧ��Ȩ�ء�
	int8* perrects_xywh;       // �����������ꡣ
} CV_cascade;
void YMCV_Load_HaarCascade(CV_cascade* cascade, const char* detctype);//���ؼ����
CVVECTORS_U16 YMCV_HaarCascade_Detect_Objects(CVIMAGE myimg, CV_cascade* cascade, float  reserved_rate);//������

float32 YMCV_Face_LBP_Hist_X2_Distance(CV_FACE_LBP_DESC lbp0, CV_FACE_LBP_DESC lbp1);//��������

//��������������
typedef struct
{
	float32* standModel;//��׼ģ��
	float32* combBases;//���ݵ����Ϸֲ�����
	float32* point_Patch;//����������patch
	int32 facepoint_num;//���������
	uint8 patch_d;//��������patch�뾶
	uint8 combmat_col;//���Ͼ�������
	float32 scale;//�������ųߴ�
	float32 facedec_offsetx;//�����������������ƫ��
	float32 facedec_offsety;
	float32 facedec_offsets;//����
}
CV_facefpoint_dector;
void YMCV_Load_FacePointDector(CV_facefpoint_dector* dector, const char* detctype);//���ؼ����
CVVECTORS_U16 YMCV_FacePoint_Detect_Objects(CVIMAGE myimg, CV_facefpoint_dector* dector, CVrect* facerect, uint8 search_r, int maxIter);//������������

#endif

