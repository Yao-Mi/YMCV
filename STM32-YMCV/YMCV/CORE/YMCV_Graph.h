#ifndef YMCV_GRAPH_H
#define YMCV_GRAPH_H

#include"../CONFIG/YMCV_PubDefine.h"
#include"../CONFIG/YMCV_Mem.h"
#include"../DEBUG/YMCV_Debug.h"
typedef struct cv_vertex
{
	struct cv_vertex* next;
	int ts; //time - stampʱ���
	int first;  //�Ӹýڵ�����Ļ���������ͷ��λ������
	float32 weight; //t-linkȨֵ
	int parent; //���ڵ㷢���Ļ�
	int dist;   //distance,�������ľ���
	uint8 t;    //ȡֵֻ����0-1��s->t����Ϊ0�� t->s����Ϊ1
}CV_vertex;//����

typedef struct cv_edge
{
	int dst;    //��ָ��Ķ���
	int next;   //ͬһ�����������һ����
	float32 weight; //n-value, ����Ȩֵ
}CV_edge;//��

typedef struct
{
	CV_vertex* vtcs;  //��㼯��
	uint32 vtcsMaxIndex;
	uint32 vtcsCurLen;

	CV_edge* edges;    //������
	uint32 edgesMaxIndex;
	uint32 edgesCurLen;
	float32 flow;   //ͼ������
}CV_graph;
typedef CV_graph* CV_GRAPH;

CV_GRAPH YMCV_Graph_Create(uint32 vtxCount, uint32 edgeCount);
void YMCV_Graph_Free(CV_GRAPH pthis);
int YMCV_Graph_AddVtx(CV_GRAPH pthis);
void YMCV_Graph_AddTermWeights(CV_GRAPH pthis, uint32 i, float32 sourceW, float32 sinkW);
void YMCV_Graph_AddEdges(CV_GRAPH pthis, uint32 i, uint32 j, float32 w, float32 revw);
float32 YMCV_Graph_MaxFlow(CV_GRAPH pthis);
uint32 YMCV_Graph_InSourceSegment(CV_GRAPH pthis, int i);

#endif // !YMCV_GRAPH_H
