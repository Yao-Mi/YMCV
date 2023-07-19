#ifndef YMCV_PUBDATA_H
#define YMCV_PUBDATA_H

#include "../CONFIG/YMCV_PubType.h"

extern const short YMCV_DisCalmb[8];
extern const int16 YMCV_NeighbPos9[9][2];//9�ھ�=����+8����
extern const int16 YMCV_Neighbour8[8][2]; //8����
extern const int8 YMCV_Search5[5];//ѭ���������������
extern const int16 YMCV_RoundSearch8[8][2];//ѭ������ r=1
extern const int16 YMCV_RoundSearch16[16][2];//ѭ������ r=2
extern const int16 YMCV_CircleSearch3x[37];//Բ������ r=3
extern const int16 YMCV_CircleSearch3y[37];
extern const int16 YMCV_CircleSearch16x[16];//��԰���� r=3
extern const int16 YMCV_CircleSearch16y[16];
extern const uint8 YMCV_CircleSearch16id[16];


extern const float32 YMCV_Gauss_Kernel[9];//gauss
extern const int16 YMCV_Sobelx_Kernel[9];//sobel
extern const int16 YMCV_Sobely_Kernel[9];
extern const int16 YMCV_Log_Kernel[9];//log
extern const int16 YMCV_Laplace_Kernel[9];//������˹�˲���

extern const uint16 YMCV_Labf_table[2041];//labӳ�亯��
extern const uint16 YMCV_Labfn_table[1025];//lab��ӳ�亯��
extern const color24_t YMCV_HeatPalette[256];//����ͼ��ɫ��

extern const float32 YMCV_Sinx360[361];//�����Ҽ���
extern const float32 YMCV_Cosx360[361];

extern const uint8 YMCV_LBP_Uniform_Pattern[256];//LBP�ȼ�ģʽ

#endif
