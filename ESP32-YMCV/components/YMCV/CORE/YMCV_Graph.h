#ifndef YMCV_GRAPH_H
#define YMCV_GRAPH_H

#include"../CONFIG/YMCV_PubDefine.h"
#include"../CONFIG/YMCV_Mem.h"
#include"../DEBUG/YMCV_Debug.h"
typedef struct cv_vertex
{
	struct cv_vertex* next;
	int ts; //time - stamp时间戳
	int first;  //从该节点出发的弧集，队列头的位置索引
	float32 weight; //t-link权值
	int parent; //父节点发出的弧
	int dist;   //distance,到树根的距离
	uint8 t;    //取值只能是0-1，s->t方向为0， t->s方向为1
}CV_vertex;//顶点

typedef struct cv_edge
{
	int dst;    //弧指向的顶点
	int next;   //同一个点出发的下一条弧
	float32 weight; //n-value, 弧的权值
}CV_edge;//边

typedef struct
{
	CV_vertex* vtcs;  //结点集合
	uint32 vtcsMaxIndex;
	uint32 vtcsCurLen;

	CV_edge* edges;    //弧集合
	uint32 edgesMaxIndex;
	uint32 edgesCurLen;
	float32 flow;   //图的流量
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
