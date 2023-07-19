#include "../SDL_LCD/SDL_LCD.h"
#include"../YMCV/CORE/YMCV_PubData.h"
#include"../YMCV/CORE/YMCV_FastMath.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"


void printComplex(float32* real, float32* imag, uint32 len, uint8* tips)
{
	printf((const char*)tips);
	if (real)
	{
		//结果显示
		printf("\n实部：");
		for (int i = 0; i < len; i++)
			printf("%f,", real[i]);
	}

	if (imag)
	{
		printf("\n虚部：");
		for (int i = 0; i < len; i++)
			printf("%f,", imag[i]);
	}
	printf("\n\n");
}


int main()
{

	float32 data[11] = { 3.900000f,-0.913114f,0.732225f,6.333200f,7.778681f,2.574413f,-2.688735f,-1.417364f,3.901729f,5.169487f,-0.100732f};

	//创建FFT渲染器
	CVFFT_RENDER fftcal = YMCV_Creat_FFT_Render_Creat(11, 1, 0);//fft 数据间隔为1个float  不转为模长夹角
	int fftLen = fftcal->unum;//得到fft处理的长度

	float32* realout = CV_malloc0(fftLen * sizeof(float32));//实部
	float32* imagout = CV_malloc0(fftLen * sizeof(float32));//虚部
	float32* realresult = CV_malloc0(fftLen * sizeof(float32));
	float32* imagresult = CV_malloc0(fftLen * sizeof(float32));
	

	printComplex(data, NULL, fftcal->danum, "原始数据");
	printf("原始数据长度：%d，fft处理长度：%d\n\n", fftcal->danum, fftcal->unum);
	// FFT 变换
	YMCV_FFT_Data_Cal(fftcal, data, NULL, realout, imagout);//只有实部
	printComplex(realout, imagout, fftLen, "FFT变换结果");

	//FFT - Shift 频移
    YMCV_FFTShift_Data_Cal(fftcal, realout, imagout, realresult, imagresult);// FFT - Shift
	printComplex(realresult, imagresult, fftLen, "FFT-Shift变换结果");

	//数据以0频率为对称 左右共轭(实部相同，虚部相反)
	int fftLen_2 = fftLen / 2;
	printf("0频率强度为:%f\n\n",YMCV_Sqrt(realresult[fftLen_2]* realresult[fftLen_2] + imagresult[fftLen_2] * imagresult[fftLen_2]));

	//FFT - iShift
	YMCV_FFTShift_Data_Cal(fftcal, realresult, imagresult, realout, imagout);// FFT - iShift
	printComplex(realout, imagout, fftLen, "FFT-iShift变换结果");

	// IFFT变换
	YMCV_IFFT_Data_Cal(fftcal, realout, imagout, realresult, imagresult);//输出分为实部和虚部
	printComplex(realresult, imagresult, fftLen, "IFFT变换结果");

	//CVFFT_Render 渲染器内存释放
	YMCV_Free_FFT_RenderFree(fftcal);
}