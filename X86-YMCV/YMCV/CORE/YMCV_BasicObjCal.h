#ifndef YMCV_BASICOBJCAL_H
#define YMCV_BASICOBJCAL_H
#include"../OPOBJ/YMCV_OBJ.h"


uint8 YMCV_Line_Clip_Liang_Barsky(CVline* thisline, int16 x, int16 y, int16 w, int16 h);//ֱ�ߴ��ڼ���
CVVECTORS_U16 YMCV_Bresenham_TraceLine(int16 x1, int16 y1, int16 x2, int16 y2);//ֱ����������
int32 YMCV_Distance_PonitToLineSegment(int16 px, int16 py, CVline* L0);//p�㵽�߶�L0�ľ������
void YMCV_Merge_LineSegments(CVVECTORS_U16 lines, uint16 disThresh, uint16 angerErr);//ֱ�ߺϲ�
CVVECTORS_U16 YMCV_Merge_Rectangles(CVVECTORS_U16 rectList);//���κϲ�
CVVECTORS_F32 YMCV_DelauneyTriangle_Construct(CVVECTORS_F32 fpointList);//�����δ���

#endif // !YMCV_BASICOBJCAL_H

