#include "./SDL_LCD.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#include <pthread.h>
#include <signal.h>


#define printf SDL_Log

// RGB565提取三通道
#define RGB565GET_R(rgb565) ((rgb565 & 0xF800) >> 8)
#define RGB565GET_G(rgb565) ((rgb565 & 0x7E0) >> 3)
#define RGB565GET_B(rgb565) ((rgb565 & 0x001F) << 3)

uint32 LCD_W, LCD_H;
color24_t LCD_POINT_COLOR = {255, 255, 255}, LCD_BCAK_COLOR = {0, 0, 0};

// SDL参数
TTF_Font *SDL_font_ttf = NULL;
SDL_Window *SDL_Win_LCD = NULL;
SDL_Renderer *SDL_Win_Renderer = NULL;
SDL_Thread *SDL_thread_process = NULL;
SDL_Event SDL_Event_Val;

static int mouse_x, mouse_y;
static int keyflg = 0, keyval;
static int SDL_isRun = 0,SDL_Render_isUsed = 0;
static int SDL_texsu = 0;//纹理
int render_thread(void *data)
{
  static int msupdate = 0;
  while (1)
  {
    // 等待5ms
    SDL_Delay(5); //取消点是指线程执行期间的一些特定位置，比如函数调用、阻塞等待等。
                  //如果线程没有设置取消点，pthread_cancel函数可能无法取消线程。
    msupdate++;

    // 查询一次事件
    if (SDL_PollEvent(&SDL_Event_Val) < 0)
    {
      SDL_Log("SDL_WaitEvent error: %s", SDL_GetError());
      break;
    }
    switch (SDL_Event_Val.type)
    {
    case SDL_MOUSEMOTION: // 鼠标移动
      mouse_x = SDL_Event_Val.motion.x;
      mouse_y = SDL_Event_Val.motion.y;
      break;
    case SDL_MOUSEBUTTONDOWN: // 鼠标按下
      break;
    case SDL_MOUSEBUTTONUP: // 鼠标放开
      break;
    case SDL_KEYDOWN: // 按键按下
      keyflg = 1;
      // char* key_str = SDL_GetKeyName(SDL_Event_Val.key.keysym.sym); // 按键的键值名称
      // keyval = key_str[0]; //取首字母
      keyval = SDL_Event_Val.key.keysym.sym;//按键的键值
      break;
    case SDL_QUIT: // 退出,不再需要刷新了
      SDL_isRun = 0;//停止现存运行
      LCD_Destory();//销毁窗口
      exit(0);
    default:
      break;
    }

    //150ms
    if(msupdate >= 25)
    {
      // 刷新渲染器
      if(SDL_Render_isUsed == 0)
      {
        SDL_Render_isUsed = 1;
        while (SDL_Render_isUsed < 5)
        {
          // 等待5ms
          SDL_Delay(1); 
          SDL_Render_isUsed ++;    
        }
        ////如果下面的 SDL_RenderPresent 发生异常，就打开上面那一段的注释吧
        SDL_RenderPresent(SDL_Win_Renderer);
        SDL_Render_isUsed = 0;
        
        msupdate = 0;
      }
    }
  }
  return 0;
}

//互斥锁
#define  SDL_inWait()   while (SDL_Render_isUsed != 0){};/*等待被使用结束*/\
                          SDL_Render_isUsed =1;/*开始使用*/

#define  SDL_UseOver()   SDL_Render_isUsed = 0;//结束使用


// 初始化窗口 
void LCD_Init(uint32 width, uint32 height)
{
  // 初始化 SDL2
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("Unable to initialize SDL2: %s", SDL_GetError());
    exit(1);
  }
  // 初始化 SDL_TTF字体
  if (TTF_Init() < 0)
  {
    SDL_Log("Error initializing SDL_ttf: %s", TTF_GetError());
    exit(1);
  }
  // 加载字体
  SDL_font_ttf = TTF_OpenFont("/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc", 16); // fonts-wqy-zenhei
  if (!SDL_font_ttf)
  {
    SDL_Log("Failed to load font:  %s", TTF_GetError());
    SDL_Log("请选择系统目录 /usr/share/fonts/truetype/下存在的ttf字体");
    exit(1);
  }

  // 创建窗口和渲染器
  SDL_Win_LCD = SDL_CreateWindow("YMCV-Ubuntu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  SDL_Win_Renderer = SDL_CreateRenderer(SDL_Win_LCD, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // 创建处理线程
  SDL_thread_process = SDL_CreateThread(render_thread, "RenderThread", NULL);
  SDL_isRun = 1;

  // LCD参数
  LCD_W = width;
  LCD_H = height;
}

// 注销窗口
void LCD_Destory()
{
  //还在运行，主动杀死线程
  if (SDL_isRun)
  {
    //避免正在刷新时杀死造成死锁
    SDL_inWait();

    pthread_t threadId = SDL_GetThreadID(SDL_thread_process);
    pthread_cancel(threadId); //发送取消请求
    // 等待线程结束
    //SDL_WaitThread(SDL_thread_process, NULL);
    pthread_join(threadId, NULL);
    
    SDL_UseOver();
  }

  // 关闭 SDL_ttf拓展
  TTF_CloseFont(SDL_font_ttf);

  SDL_inWait();
  // 释放资源
  SDL_DestroyRenderer(SDL_Win_Renderer);
  SDL_UseOver();
  
  //等待纹理销毁完成
  while (SDL_texsu != 0)
  {
    SDL_Delay(10);
  }

  //销毁窗口
  SDL_DestroyWindow(SDL_Win_LCD);
  //退出
  TTF_Quit();
  SDL_Quit();
}

// 设置画笔颜色
void LCD_Point_Color_Set(color24_t thiscolor)
{
  SDL_inWait();

  LCD_POINT_COLOR = thiscolor;
  SDL_SetRenderDrawColor(SDL_Win_Renderer, thiscolor.R, thiscolor.G, thiscolor.B, 255);
  
  SDL_UseOver();
}

void setcolor(uint16 rgb565)
{
  SDL_inWait();

  LCD_POINT_COLOR = (color24_t){.R = RGB565GET_R(rgb565), .G = RGB565GET_G(rgb565), .B = RGB565GET_B(rgb565)};
  SDL_SetRenderDrawColor(SDL_Win_Renderer, LCD_POINT_COLOR.R, LCD_POINT_COLOR.G, LCD_POINT_COLOR.B, 255);
    
  SDL_UseOver();
}

color24_t LCD_Pen_Color_Get()
{
  return LCD_POINT_COLOR;
}

void LCD_Draw_Line(uint32 x1, uint32 y1, uint32 x2, uint32 y2)
{
  SDL_inWait();

  // 画一条直线
  SDL_RenderDrawLine(SDL_Win_Renderer, x1, y1, x2, y2);
    
  SDL_UseOver();
}
// 画圆
void LCD_Draw_Circle(uint32 x, uint32 y, uint32 r)
{
  SDL_inWait();
  
  int x0 = r, y0 = 0;
  int d = 1 - r;
  while (x0 > y0)
  {
    // 填充圆
    //  if (fill) {
    //      for (int i = x - x0; i <= x + x0; i++) {
    //          SDL_RenderDrawPoint(renderer, i, y + y0);
    //          SDL_RenderDrawPoint(renderer, i, y - y0);
    //      }
    //      for (int i = x - y0; i <= x + y0; i++) {
    //          SDL_RenderDrawPoint(renderer, i, y + x0);
    //          SDL_RenderDrawPoint(renderer, i, y - x0);
    //      }
    //  } else
    {
      SDL_RenderDrawPoint(SDL_Win_Renderer, x + x0, y + y0);
      SDL_RenderDrawPoint(SDL_Win_Renderer, x + y0, y + x0);
      SDL_RenderDrawPoint(SDL_Win_Renderer, x - y0, y + x0);
      SDL_RenderDrawPoint(SDL_Win_Renderer, x - x0, y + y0);
      SDL_RenderDrawPoint(SDL_Win_Renderer, x - x0, y - y0);
      SDL_RenderDrawPoint(SDL_Win_Renderer, x - y0, y - x0);
      SDL_RenderDrawPoint(SDL_Win_Renderer, x + y0, y - x0);
      SDL_RenderDrawPoint(SDL_Win_Renderer, x + x0, y - y0);
    }
    y0++;
    if (d < 0)
    {
      d += 2 * y0 + 1;
    }
    else
    {
      x0--;
      d += 2 * (y0 - x0 + 1);
    }
  }
    
  SDL_UseOver();
}

// 画点
void LCD_Draw_Point(uint32 x, uint32 y, color24_t thiscolor)
{
  color24_t oldclr = LCD_Pen_Color_Get();

  SDL_inWait();
  
  SDL_SetRenderDrawColor(SDL_Win_Renderer, thiscolor.R, thiscolor.G, thiscolor.B, 255);
  SDL_RenderDrawPoint(SDL_Win_Renderer, x, y);
  SDL_SetRenderDrawColor(SDL_Win_Renderer, oldclr.R, oldclr.G, oldclr.B, 255);// 颜色重置

  SDL_UseOver();
}
// 暂停等待按键
void PAUSE()
{
  while (keyflg == 0)
  {
  };
  keyflg = 0;
}
// 延时
void Delay(int ms)
{
  SDL_Delay(ms);
}
// 现实字符串
void LCD_ShowString(uint16 x, uint16 y, const char *p)
{
  SDL_inWait();

  SDL_texsu = 1;//开始准备纹理 
  // 在窗口中写字
  SDL_Color textColor = {.r = LCD_POINT_COLOR.R, .g = LCD_POINT_COLOR.G, .b = LCD_POINT_COLOR.B, .a = 255};
  SDL_Color bckColor = {.r = LCD_BCAK_COLOR.R, .g = LCD_BCAK_COLOR.G, .b = LCD_BCAK_COLOR.B, .a = 255};
  //TTF_RenderUTF8_Blended
  SDL_Surface *textSurface = TTF_RenderUTF8_Shaded(SDL_font_ttf, p, textColor,bckColor);//utf8编码

  
  if (textSurface != NULL)
  {
    // 生成文理
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(SDL_Win_Renderer, textSurface);
    if (textTexture != NULL)
    {
      SDL_Rect textRect = {.x = x, .y = y, .w = textSurface->w, .h = textSurface->h};
      // 写入缓冲
      SDL_RenderCopy(SDL_Win_Renderer, textTexture, NULL, &textRect);
      // 釋放資源
      SDL_DestroyTexture(textTexture);
    }
    // 釋放資源
    SDL_FreeSurface(textSurface);
  }

  SDL_texsu = 0;//纹理已经销毁
    
  SDL_UseOver();
}

// 获取鼠标的坐标
int LCD_GetXY(int startx, int starty, int rew, int reh, int *px, int *py)
{
  uint16 cx = mouse_x - startx;
  uint16 cy = mouse_y - starty;
  int geflag = 0;
  if ((cx < rew) && (cy < reh))
    geflag = 1;

  if (geflag)
  {
    *px = cx;
    *py = cy;
  }
  return geflag;
}

// 获取键盘输入
int LCD_GetChar(char *c)
{
  if (keyflg)
  {
    *c = keyval;
    keyflg = 0; // 清除

    return 1;
  }
  return 0;
}

// 清屏
void LCD_Clear(uint16 Color)
{
  color24_t oldclr = LCD_Pen_Color_Get();
  // 设置颜色
  color24_t thiscolor = {.R = RGB565GET_R(Color), .G = RGB565GET_G(Color), .B = RGB565GET_B(Color)};
  LCD_Point_Color_Set(thiscolor);

  SDL_inWait();

  uint16 i, j;
  for (i = 0; i < LCD_W; i++)
  {
    for (j = 0; j < LCD_H; j++)
    {
      SDL_RenderDrawPoint(SDL_Win_Renderer, i, j);
    }
  }
    
  SDL_UseOver();

  // 颜色重置
  LCD_Point_Color_Set(oldclr);
}

void LCD_Fill_RgbRect(int startx, int starty, int width, int height, color24_t *data)
{
  
  uint16 i, j;
  color24_t thiscolor;
  color24_t oldclr = LCD_Pen_Color_Get();

  for (i = 0; i < width; i++)
  {
    for (j = 0; j < height; j++)
    {
      thiscolor = data[i * width + j];
      LCD_Draw_Point(startx + i, starty + j,thiscolor);
    }
  }

  // 颜色重置
  LCD_Point_Color_Set(oldclr);
}

void LCD_Fill_GrayRect(int startx, int starty, int width, int height, _color8_t *data)
{
  uint16 i, j;
  color24_t thiscolor;
  color24_t oldclr = LCD_Pen_Color_Get();
  for (i = 0; i < width; i++)
  {
    for (j = 0; j < height; j++)
    {
      _color8_t cl8v = data[i * width + j];
      thiscolor = (color24_t){.R = cl8v, .G = cl8v, .B = cl8v};
      
      LCD_Draw_Point(startx + i, starty + j,thiscolor);
    }
  }
  // 颜色重置
  LCD_Point_Color_Set(oldclr);
}

void LCD_Fill_ConstRgbRect(int startx, int starty, int width, int height, color24_t thiscolor)
{
  
  color24_t oldclr = LCD_Pen_Color_Get();
  // 设置颜色
  LCD_Point_Color_Set(thiscolor);

  SDL_inWait();

  uint16 i, j;
  for (i = 0; i < width; i++)
  {
    for (j = 0; j < height; j++)
    {
      SDL_RenderDrawPoint(SDL_Win_Renderer, startx + i, starty + j);
    }
  }
    
  SDL_UseOver();

  // 颜色重置
  LCD_Point_Color_Set(oldclr);
}

void LCD_Draw_Rect(int startx, int starty, int width, int height, color24_t thiscolor)
{
  
  color24_t oldclr = LCD_Pen_Color_Get();
  // 设置颜色
  LCD_Point_Color_Set(thiscolor);

  SDL_inWait();
  uint16 i, j;

  for (i = 0; i < height; i++)
  {
    j = 0;
    SDL_RenderDrawPoint(SDL_Win_Renderer, startx + j, starty + i);
  }

  for (i = 0; i < height; i++)
  {
    j = width;
    SDL_RenderDrawPoint(SDL_Win_Renderer, startx + j, starty + i);
  }

  for (j = 0; j < width; j++)
  {
    i = height;
    SDL_RenderDrawPoint(SDL_Win_Renderer, startx + j, starty + i);
  }
  for (j = 0; j < width; j++)
  {
    i = 0;
    SDL_RenderDrawPoint(SDL_Win_Renderer, startx + j, starty + i);
  }
    
  SDL_UseOver();
  // 颜色重置
  LCD_Point_Color_Set(oldclr);
}

//				RGB565格式图像在LCD上显示
void show_rgb(uint16 x, uint16 y, uint16 Width, uint16 Height, uint16 *pic)
{
  
  uint16 i, j, rgb565;
  color24_t oldclr = LCD_Pen_Color_Get();
  
  for (i = 0; i < Height; i++)
  {
    for (j = 0; j < Width; j++)
    {
      rgb565 = *(pic + i * Width + j);
      // 设置颜色
      color24_t thiscolor = {.R = RGB565GET_R(rgb565), .G = RGB565GET_G(rgb565), .B = RGB565GET_B(rgb565)};

      LCD_Draw_Point(x + j, y + i,thiscolor);
    }

    // 颜色重置
    LCD_Point_Color_Set(oldclr);
  }
}

void show_gray(uint16 x, uint16 y, uint16 Width, uint16 Height, uint8 *pic)
{
  color24_t oldclr = LCD_Pen_Color_Get();

  uint16 i, j;
  uint8 gray;
  for (i = 0; i < Height; i++)
  {
    for (j = 0; j < Width; j++)
    {
      gray = *(pic + i * Width + j);
      // 设置颜色
      color24_t thiscolor = (color24_t){.R = gray, .G = gray, .B = gray};

      LCD_Draw_Point(x + j, y + i,thiscolor);
    }
  }
  // 颜色重置
  LCD_Point_Color_Set(oldclr);
}

void LCD_show_image(uint16 x, uint16 y, CVIMAGE myimg)
{
  if (myimg->type == CVRgb16Type)
  {
    show_rgb(x, y, myimg->width, myimg->height, (uint16 *)myimg->data);
  }

  else if (myimg->type <= CVGrayType)
  {
    show_gray(x, y, myimg->width, myimg->height, (uint8 *)myimg->data);
  }
}

#include <stdio.h>
#include <stdlib.h>
void LCDChannelTransformShow(CVPIX_RENDER myRender, int startx, int starty, int width)
{
  int i, endx, endy, j, k, my;
  float deta = (width - 1) / 255.0f;
  int32 num, calnum, channelnum = 1;
  uint8 *showHis, *thisshowHis, *RenderTrTable, *thisRenderTrTable;
  uint8 *myPic = 0;
  RenderTrTable = myRender->PixTrTable;

  if (myRender->ChannelNum == PIXChannelNum3)
  {
    channelnum = 3;
  }
  else if (myRender->ChannelNum == PIXChannelNum1)
  {
    channelnum = 1;
  }

  showHis = (uint8 *)malloc(channelnum * width * sizeof(uint8)); // malloc 0

  for (int32 k = 0; k < channelnum; k++)
  {
    thisshowHis = &showHis[k * width];
    thisRenderTrTable = &RenderTrTable[k * 256];
    j = 0;
    for (i = 0; i < width; i++)
    {
      if (j < (i * 255) / (width - 1))
        j++;
      calnum = thisRenderTrTable[j];
      num = 1; // num
      while (j < (i * 255) / (width - 1))
      {
        j++;
        num++;
        calnum += thisRenderTrTable[j];
      }

      calnum /= num; // 平均一下
      thisshowHis[i] = (uint8)((calnum > 255) ? 255u : ((calnum < 0) ? 0u : calnum));
    }
  }
  // 申请缓冲画布
  myPic = (uint8 *)malloc(width * width * channelnum * sizeof(uint8)); // malloc 1
  memset(myPic, 0, width * width * channelnum * sizeof(uint8));        // 填充区域

  // 外接矩形框
  for (k = 0; k < channelnum; k++)
  {
    endx = (width - 1) * channelnum;
    endy = channelnum * width;
    my = (width - 1) * channelnum * width;
    for (i = 0; i < width; i++)
    {
      // 灰色
      myPic[i * endy + 0 * channelnum + k] = (uint8)80; // 垂直线
      myPic[i * endy + endx + k] = (uint8)80;
      myPic[0 * endy + i * channelnum + k] = (uint8)80; // 水平线
      myPic[my + i * channelnum + k] = (uint8)80;       // 水平线
    }
  }

  // 清空区域,填充矩形框
  for (k = 0; k < channelnum; k++)
  {
    thisshowHis = &showHis[k * width]; // 处理其中某个通道
    for (i = 0; i < width; i++)
    {
      my = (thisshowHis[i] * deta);
      myPic[(width - 1 - my) * channelnum * width + i * channelnum + k] = (uint8)255;
      // endx = (thisshowHis[i+1] * deta);
      // line(startx + i, starty - endy, startx + i+1, starty - endx);
    }
  }
  // 显示区域
  if (myRender->ChannelNum == PIXChannelNum3)
  {
    LCD_Fill_RgbRect(startx, starty - width, width, width, (color24_t *)myPic);
  }
  else if (myRender->ChannelNum == PIXChannelNum1)
  {
    LCD_Fill_GrayRect(startx, starty - width, width, width, (_color8_t *)myPic);
  }

  setcolor(BLACK); // DARKGRAY

  endx = startx + width;
  endy = starty - width;

  // line(startx, starty, endx, starty);//上水平线
  // line(startx, endy, endx, endy);
  // line(startx, starty, startx, endy);//左垂直线
  // line(endx, starty, endx, endy);

  // 文字
  setcolor(WHITE);
  LCD_ShowString(startx - 8, starty - 8, "0");
  LCD_ShowString(startx - 8, endy - 8, "255");
  LCD_ShowString(endx - 8, starty - 8, "255");

  free(myPic);
  free(showHis); // 释放申请内存
}

void LCDHistShow(int32 *myHis, PIXChannelNum channelNum, int startx, int starty, int width)
{
  int i, endx, endy, j, k, maxH, my;
  float deta = (width - 1) / 255.0, detakh;
  int32 num, calnum, channelnum = 1, *thisHis;
  uint8 *showHis, *thisshowHis, lasth;
  uint8 *myPic = 0;

  if (channelNum == PIXChannelNum3)
  {
    channelnum = 3;
  }
  else if (channelNum == PIXChannelNum1)
  {
    channelnum = 1;
  }
  showHis = (uint8 *)malloc(width * channelnum * sizeof(uint8));

  for (int32 k = 0; k < channelnum; k++)
  {
    // 进行放缩
    thisshowHis = &showHis[k * width];
    thisHis = &myHis[k * 256];

    // 找到最大值
    maxH = 0;
    for (i = 0, j = 256; i < j; i++)
    {
      if (maxH < thisHis[i])
        maxH = thisHis[i];
    }
    detakh = 255.0 / maxH;

    j = 0;
    for (i = 0; i < width; i++)
    {
      if (j < (i * 255) / (width - 1))
        j++;
      calnum = (thisHis[j] * detakh);
      num = 1; // num
      while (j < (i * 255) / (width - 1))
      {
        j++;
        num++;
        calnum += (thisHis[j] * detakh);
      }
      calnum /= num; // 平均一下
      thisshowHis[i] = (uint8)((calnum > 255) ? 255u : ((calnum < 0) ? 0u : calnum));
    }
  }
  // 申请缓冲画布
  myPic = (uint8 *)malloc(width * width * channelnum * sizeof(uint8)); // malloc 1
  memset(myPic, 0, width * width * channelnum * sizeof(uint8));        // 填充区域

  // 外接矩形框
  for (k = 0; k < channelnum; k++)
  {
    endx = (width - 1) * channelnum;
    endy = channelnum * width;
    my = (width - 1) * channelnum * width;
    for (i = 0; i < width; i++)
    {
      // 灰色
      myPic[i * endy + 0 * channelnum + k] = (uint8)80; // 垂直线
      myPic[i * endy + endx + k] = (uint8)80;
      myPic[0 * endy + i * channelnum + k] = (uint8)80; // 水平线
      myPic[my + i * channelnum + k] = (uint8)80;       // 水平线
    }
  }

  // 清空区域,填充矩形框
  for (k = 0; k < channelnum; k++)
  {
    thisshowHis = &showHis[k * width]; // 处理其中某个通道
    for (i = 0; i < width; i++)
    {
      my = (thisshowHis[i] * deta);
      for (j = 0; j <= my; j++)
      {
        myPic[(width - 1 - j) * channelnum * width + i * channelnum + k] = (uint8)255;
      }
    }
  }
  // 显示区域
  if (channelNum == PIXChannelNum3)
  {
    LCD_Fill_RgbRect(startx, starty - width, width, width, (color24_t *)myPic);
  }
  else if (channelNum == PIXChannelNum1)
  {
    LCD_Fill_GrayRect(startx, starty - width, width, width, (_color8_t *)myPic);
  }
  setcolor(BLACK); // DARKGRAY

  endx = startx + width;
  endy = starty - width;

  // 文字
  setcolor(WHITE);
  LCD_ShowString(startx - 8, starty - 8, "0");
  LCD_ShowString(startx - 8, endy - 8, "max");
  LCD_ShowString(endx - 8, starty - 8, "255");

  free(myPic);
  free(showHis); // 释放申请内存
}

void LCDHogHistShow(CVVECTORS_F32 hogdescrible, int startx, int starty, int invers)
{
  int cell_size = 8;
  int lenmax = cell_size / 2;
  int sizediv2 = cell_size / 2;

  color24_t lstcolor = LCD_POINT_COLOR; // 原始的颜色
  color24_t pencolor;
  // 计算x,y方向cell的个数
  int xnum = hogdescrible->dim / (sizeof(CVhog_cell8x8) / sizeof(float32));
  int ynum = hogdescrible->dataNum;
  // 遍历cell
  for (int y = 0; y < ynum; y++)
  {
    CVHOG_CELL8X8 cell_1 = (CVHOG_CELL8X8)&hogdescrible->datap[y * hogdescrible->dim];
    for (int x = 0; x < xnum; x++)
    {
      // 最中心点
      int cx = x * cell_size + sizediv2;
      int cy = y * cell_size + sizediv2;
      // 9个维度
      for (int hi = 0; hi < 9; hi++)
      {
        // 获得线条颜色强度
        uint16 vcolor = cell_1[x].hist[hi] * 4 * 255; // 2x2的block,每个cell均值期望为0.25，这里取颜色max期望为0.75*255
        vcolor = CVLimitMaxMin(0, vcolor, 255);
        pencolor.R = vcolor;
        pencolor.G = vcolor;
        pencolor.B = vcolor;
        LCD_Point_Color_Set(pencolor);

        int x2 = lenmax * YMCV_Cos(hi * 20 * YMCV_Deg2Rad);
        int y2 = lenmax * YMCV_Sin(hi * 20 * YMCV_Deg2Rad);
        //  \ /
        //   *
        //  / \

        if (invers)
        {
          LCD_Draw_Line(startx + cx + y2, starty + cy - x2, startx + cx - y2, starty + cy + x2);
        }
        else
        {
          LCD_Draw_Line(startx + cx - x2, starty + cy - y2, startx + cx + x2, starty + cy + y2);
        }
      }
    }
  }
  // 颜色还原
  LCD_Point_Color_Set(lstcolor);
}

void LCDLbpHistShow(CV_FACE_LBP_DESC lbpdescrible, int startx, int starty)
{
  // cell大小
  int cell_size = 16;
  int lenmax = cell_size / 2;
  int sizediv2 = cell_size / 2;

  color24_t lstcolor = LCD_POINT_COLOR; // 原始的颜色
  color24_t pencolor;
  // x,y方向cell的个数,cell大小按16*16计算
  int xnum = 7;
  int ynum = 7;
  // 遍历cell
  for (int y = 0; y < ynum; y++)
  {
    uint8 *cell_1 = (uint8 *)&lbpdescrible->hist[y * 7 * 59];
    for (int x = 0; x < xnum; x++)
    {
      // 最中心点
      int cx = x * cell_size + sizediv2;
      int cy = y * cell_size + sizediv2;
      // 59个维度,每3个数据合成20度
      for (int hi = 0; hi < 18; hi++)
      {
        // 获得线条颜色强度
        uint16 vcolor = (cell_1[x * 7 + 3 * hi + 0] + cell_1[x * 7 + 3 * hi + 1] + cell_1[x * 7 + 3 * hi + 2]) * 4; // 每个通道期望为4.3，3个通道合并则为13，这里扩大到52
        vcolor = CVLimitMaxMin(0, vcolor, 255);
        pencolor.R = vcolor;
        pencolor.G = vcolor;
        pencolor.B = vcolor;
        LCD_Point_Color_Set(pencolor);

        int x2 = lenmax * YMCV_Cos(hi * 20 * YMCV_Deg2Rad);
        int y2 = lenmax * YMCV_Sin(hi * 20 * YMCV_Deg2Rad);
        //  \ /
        //-- * --

        LCD_Draw_Line(startx + cx, starty + cy, startx + cx + x2, starty + cy + y2);
      }
    }
  }
  // 颜色还原
  LCD_Point_Color_Set(lstcolor);
}

void LCDProjHistShow(CVVECTORS_U16 projhist, uint8 axis_0x_1y, int startx, int starty)
{
  color24_t lstcolor = LCD_POINT_COLOR; // 原始的颜色
  color24_t pencolor = {255, 255, 255};
  LCD_Point_Color_Set(pencolor);
  // x轴投影
  if (axis_0x_1y == 0)
  {
    for (int i = 0; i < projhist->dataNum; i++)
    {
      LCD_Draw_Line(startx + i, starty, startx + i, starty + projhist->datap[i]); // 竖线
    }
  }
  // y轴投影
  else
  {
    for (int i = 0; i < projhist->dataNum; i++)
    {
      LCD_Draw_Line(startx, starty + i, startx + projhist->datap[i], starty + i); // 横线
    }
  }
  // 颜色还原
  LCD_Point_Color_Set(lstcolor);
}

// 人脸特征点绘制
void LCD_FaceFeaturePoint_Show(CVpoint fupoint[40], int startx, int starty)
{
  color24_t lstcolor = LCD_POINT_COLOR; // 原始的颜色
  color24_t pencolor = {0, 255, 0};     // 绿色
  LCD_Point_Color_Set(pencolor);
  pencolor.R = 255; // 红色
  pencolor.G = 0;
  // 绘制点
  for (int pi = 0; pi < 40; pi++) // 40个特征点
  {
    // 特征点连线
    //  1 - 2 - 3 - 4     5 - 6 - 7 - 8 - 9 - 10 - 5
    //  11 - 12 - 13 - 14     15 - 16 - 17 - 18 - 19 - 20 - 15
    //  6 - 10  7 - 9     16 - 20  17 - 19
    //  21 - 22 - 23 - 24 - 25 - 26
    //  27 - 28 - 29 - 30 - 31 -32 - 27
    //  27 - 36 - 35 - 30   27 - 33 - 34 - 30
    //  37 - 38 - 39 - 37  39 - 40

    uint8 lineto[] = {1, 2, 2, 3, 3, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 5,
                      11, 12, 12, 13, 13, 14, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 15,
                      6, 10, 7, 9, 16, 20, 17, 19,
                      21, 22, 22, 23, 23, 24, 24, 25, 25, 26,
                      27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 27,
                      27, 36, 36, 35, 35, 30, 27, 33, 33, 34, 34, 30,
                      37, 38, 38, 39, 39, 37, 39, 40};

    // 绘制线段
    for (int pli = 0; pli < sizeof(lineto) / 2; pli++)
    {
      int id0 = lineto[2 * pli + 0] - 1;
      int id1 = lineto[2 * pli + 1] - 1;
      LCD_Draw_Line(startx + fupoint[id0].x, starty + fupoint[id0].y, startx + fupoint[id1].x, starty + fupoint[id1].y);
    }

    // 绘制特征点
    LCD_Fill_ConstRgbRect(startx + fupoint[pi].x - 1, starty + fupoint[pi].y - 1, 3, 3, pencolor);
  }
  // 颜色还原
  LCD_Point_Color_Set(lstcolor);
}


