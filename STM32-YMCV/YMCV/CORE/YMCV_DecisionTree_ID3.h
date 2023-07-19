#ifndef YMCV_DECISIONTREE_ID3_H
#define YMCV_DECISIONTREE_ID3_H

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_Mem.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "../DEBUG/YMCV_Debug.h"
#include "../OPOBJ/YMCV_Creat.h"
#include "../OPOBJ/YMCV_Free.h"

typedef struct cv_decison_tree_node_ {
	uint8 lable;//Ҷ�ڵ��ǩ
	uint8 isleave;
	uint16 dim_id;
	int16 threshold;
	struct cv_decison_tree_node_* left; //��ڵ�
	struct cv_decison_tree_node_* righ;//�ҽڵ�
}CV_decison_tree_node;
typedef CV_decison_tree_node* CV_DECISION_TREE;

//����������
CV_DECISION_TREE YMCV_DecisionTree_Creat(void);

//����������
void YMCV_DecisionTree_Free(CV_DECISION_TREE pthis);

//������ѵ��
void YMCV_DecisionTree_ID3_Trains(CV_DECISION_TREE cur_node, CVVECTORS_U16 datavecp, uint8* lable);

//�������ݾ��߱�ǩ
void YMCV_DecisionTree_Classify(CVVECTORS_U16 sample, uint8* label, CV_DECISION_TREE ID3_Dstree);

//���������
void YMCV_DecisionTree_Save(CV_DECISION_TREE pthis);
//���ؾ�����
void YMCV_DecisionTree_Load(CV_DECISION_TREE cur_node, uint8* model);

#endif // !YMCV_DECISIONTREE_ID3_H

