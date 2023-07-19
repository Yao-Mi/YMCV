#ifndef YMCV_DECISIONTREE_ID3_H
#define YMCV_DECISIONTREE_ID3_H

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_Mem.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "../DEBUG/YMCV_Debug.h"
#include "../OPOBJ/YMCV_Creat.h"
#include "../OPOBJ/YMCV_Free.h"

typedef struct cv_decison_tree_node_ {
	uint8 lable;//叶节点标签
	uint8 isleave;
	uint16 dim_id;
	int16 threshold;
	struct cv_decison_tree_node_* left; //左节点
	struct cv_decison_tree_node_* righ;//右节点
}CV_decison_tree_node;
typedef CV_decison_tree_node* CV_DECISION_TREE;

//决策树创建
CV_DECISION_TREE YMCV_DecisionTree_Creat();

//决策树销毁
void YMCV_DecisionTree_Free(CV_DECISION_TREE pthis);

//决策树训练
void YMCV_DecisionTree_ID3_Trains(CV_DECISION_TREE cur_node, CVVECTORS_U16 datavecp, uint8* lable);

//计算数据决策标签
void YMCV_DecisionTree_Classify(CVVECTORS_U16 sample, uint8* label, CV_DECISION_TREE ID3_Dstree);

//保存决策树
void YMCV_DecisionTree_Save(CV_DECISION_TREE pthis);
//加载决策树
void YMCV_DecisionTree_Load(CV_DECISION_TREE cur_node, uint8* model);

#endif // !YMCV_DECISIONTREE_ID3_H

