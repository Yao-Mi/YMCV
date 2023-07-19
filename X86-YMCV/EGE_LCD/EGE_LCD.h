#pragma once
#include"../YMCV/CONFIG/YMCV_PubType.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "../YMCV/OPOBJ/YMCV_OBJ.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"

void LCD_Init(uint32 width, uint32 height);
void LCD_Destory();
void LCD_Point_Color_Set(color24_t thiscolor);
color24_t LCD_Pen_Color_Get();
void LCD_Draw_Point(uint32 x, uint32 y, color24_t thiscolor);
void LCD_Draw_Circle(uint32 x, uint32 y, uint32 r);
void LCD_Draw_Line(uint32 x1, uint32 y1, uint32 x2, uint32 y2);

int LCD_GetXY(int startx, int starty, int rew, int reh, int* px, int* py);
int LCD_GetChar(char* c);
void PAUSE();
void Delay(int ms);

void LCD_show_image(uint16 x, uint16 y, CVIMAGE myimg);
void LCD_Clear(int Color);
void LCD_ShowString(uint16 x, uint16 y, const char* p);
void LCDChannelTransformShow(CVPIX_RENDER myRender, int startx, int starty, int width);//显示渲染通道
void LCDHistShow(int32* myHis, PIXChannelNum channelNum, int startx, int starty, int width);//显示直方图
void LCDHogHistShow(CVVECTORS_F32 hogdescrible, int startx, int starty, int invers);//hog特征描述显示
void LCDLbpHistShow(CV_FACE_LBP_DESC lbpdescrible, int startx, int starty);//lbp特征直方图描述面试
void LCDProjHistShow(CVVECTORS_U16 projhist, uint8 axis_0x_1y, int startx, int starty);//投影直方图显示
void LCD_FaceFeaturePoint_Show(CVpoint fupoint[40], int startx, int starty);//人脸特征点

void LCD_Fill_ConstRgbRect(int startx, int starty, int width, int height, color24_t thiscolor);
void LCD_Draw_Rect(int startx, int starty, int width, int height, color24_t thiscolor);

#ifdef __cplusplus
}
#endif // __cplusplus

