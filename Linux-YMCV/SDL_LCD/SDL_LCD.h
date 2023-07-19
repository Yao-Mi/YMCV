#pragma once
#include "../YMCV/CONFIG/YMCV_PubType.h"

#ifdef __cplusplus
extern "C"
{
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

  int LCD_GetXY(int startx, int starty, int rew, int reh, int *px, int *py);
  int LCD_GetChar(char *c);
  void PAUSE();
  void Delay(int ms);

  void LCD_show_image(uint16 x, uint16 y, CVIMAGE myimg);
  void LCD_Clear(uint16 Color);
  void LCD_ShowString(uint16 x, uint16 y, const char *p);
  void LCDChannelTransformShow(CVPIX_RENDER myRender, int startx, int starty, int width);      // 显示渲染通道
  void LCDHistShow(int32 *myHis, PIXChannelNum channelNum, int startx, int starty, int width); // 显示直方图
  void LCDHogHistShow(CVVECTORS_F32 hogdescrible, int startx, int starty, int invers);         // hog特征描述显示
  void LCDLbpHistShow(CV_FACE_LBP_DESC lbpdescrible, int startx, int starty);                  // lbp特征直方图描述面试
  void LCDProjHistShow(CVVECTORS_U16 projhist, uint8 axis_0x_1y, int startx, int starty);      // 投影直方图显示
  void LCD_FaceFeaturePoint_Show(CVpoint fupoint[40], int startx, int starty);                 // 人脸特征点

  void LCD_Fill_ConstRgbRect(int startx, int starty, int width, int height, color24_t thiscolor);
  void LCD_Draw_Rect(int startx, int starty, int width, int height, color24_t thiscolor);

// 鐢荤瑪棰滆壊
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 // 妫曡壊
#define BRRED 0XFC07 // 妫曠孩鑹?
#define GRAY 0X8430  // 鐏拌壊
  // GUI棰滆壊

#define DARKBLUE 0X01CF  // 娣辫摑鑹?
#define LIGHTBLUE 0X7D7C // 娴呰摑鑹?
#define GRAYBLUE 0X5458  // 鐏拌摑鑹?
  // 浠ヤ笂涓夎壊涓篜ANEL鐨勯?滆壊

#define LIGHTGREEN 0X841F // 娴呯豢鑹?
#define LGRAY 0XC618      // 娴呯伆鑹?(PANNEL),绐椾綋鑳屾櫙鑹?

#define LGRAYBLUE 0XA651 // 娴呯伆钃濊壊(涓?闂村眰棰滆壊)
#define LBBLUE 0X2B12    // 娴呮?曡摑鑹?(閫夋嫨鏉＄洰鐨勫弽鑹?)

#ifdef __cplusplus
}
#endif // __cplusplus
