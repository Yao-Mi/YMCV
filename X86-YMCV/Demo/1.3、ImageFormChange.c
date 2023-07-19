#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_FormCge.h"
/*******************************************************************************************
	*
	* 说明  该例程展示了 图片创建，以及图片的格式转换
	*
	***************************************************************************************/
int main()
{
	LCD_Init(1000, 480);

	//为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
	uint8* myData = CV_malloc1(sizeof(linglong00_gray));//必须采用CV_malloc1为图片数据申请内存
	CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
	CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//灰度图
	
	//显示图片0
	LCD_show_image(0, 0, myImage0);
	//暂停
	PAUSE();

	//新图片创建，内容为空(默认不进行初始化，内容为随机值)
	CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);

	//转二值点图
	YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_GrayToDotBinary);
	//显示图片1
	LCD_show_image(500, 0, myImage1);
	//暂停
	PAUSE();


	//使用完图片，要释放图片内存
	YMCV_Free_ImgFree(myImage0);
	YMCV_Free_ImgFree(myImage1);


	PAUSE();
	LCD_Destory();
	return 0;
}



