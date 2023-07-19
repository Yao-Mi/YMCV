#ifndef _YMCV_OBJ_H
#define _YMCV_OBJ_H
#include"../CONFIG/YMCV_PubType.h"

//ͼ��
typedef enum
{
	CVUnDefineType = 0,
	CVBinaryType = 1,// ��ֵͼ
	CVTrimapType,//��ɫͼ
	CVGrayType,//�Ҷ�ͼ
	CVRgb16Type,//RGB565
	CVFGrayType,//float gray
	CVIGrayType,//int gray
	CVCGrayType,//complex gray
}CVImgType;

//ͼ��
typedef struct 
{
	uint16 width;
	uint16 height;
	uint8 pmlc; //ͼ����ɱ�ʶ
	size_t sizet;
	CVImgType type;
	_color8_t* data;
}CVimage;
typedef CVimage* CVIMAGE;

//�����
typedef struct
{
	uint16 x, y;
}CVpoint;
//����
typedef struct
{
	uint16 x, y, w, h;
}CVrect;
//�߶�
typedef struct
{
	int16 x1;
	int16 y1;
	int16 x2;
	int16 y2;
}CVline;

//��
typedef struct
{
	float x;
	float y;
}CVfpoint;

//�߶�
typedef struct
{
	CVfpoint pa;
	CVfpoint pb;
}CVfline;

//������
typedef struct
{
	CVfpoint A, B, C;
}CVftriangle;

//����
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

//����
typedef struct
{
	float32* datap;
	uint16 row;
	uint16 col;
}CVfmat;
typedef CVfmat* CVFMAT;

//����
typedef struct
{
	CVVECTORS_U16* chain;
	uint32 cNum;//������
}CVchains_u16;
typedef CVchains_u16* CVCHAINS_U16;

//����LBP�ȼ�ģʽֱ��ͼ
typedef struct
{
	uint8 hist[7 * 7 * 59];// ����Ϊ 7��7��С ,�ȼ�ģʽ59��
} CV_face_lbp_desc;
typedef CV_face_lbp_desc* CV_FACE_LBP_DESC;

//Ŀ�������
typedef struct
{
	CVrect roi;
	uint16 histnum;
	float32* hist;
	float32* Kdis;
}CV_target_track;
typedef CV_target_track* CV_TARGET_TRACK;

 //����״̬�ռ���� 
typedef struct { 
	int x;//x����λ�� 
	int y;//x����λ�� 
	int Hx;//x����봰�� 
	int Hy;//y����봰�� 
	float vx;//x�����˶��ٶ� 
	float vy;//y�����˶��ٶ� 
	float vs;//�߶ȱ任�ٶ� 
} CV_particle_state;

//���Ӹ�����
typedef struct
{
	uint8 particleNum;//���Ӹ���
	uint16 histNum;//ֱ��ͼ����
	CVrect result;
	float32 velocity_delta;//�ٶ��Ŷ���ֵ
	float32 scale_delta;//�߶ȱ任�ٶ��Ŷ�����
	CV_particle_state* states;
	float32* weights;
	float32* hists;
}CV_particle_track;
typedef CV_particle_track* CV_PARTICLE_TRACK;

#endif

