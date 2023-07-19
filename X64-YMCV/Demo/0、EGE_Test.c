#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"

int main()
{
    LCD_Init(640, 480);
    color24_t pointcolor = { .R=255,.G = 255 ,.B=0};

    LCD_Point_Color_Set(pointcolor);
    LCD_Draw_Circle(320, 240, 100);	// »­Ô²£¬Ô²ÐÄÎª(320, 240)£¬°ë¾¶Îª100
    PAUSE();

    LCD_Destory();
    return 0;
}




