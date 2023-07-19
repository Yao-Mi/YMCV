#include "../EGE_LCD/EGE_LCD.h"
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
		//�����ʾ
		printf("\nʵ����");
		for (int i = 0; i < len; i++)
			printf("%f,", real[i]);
	}

	if (imag)
	{
		printf("\n�鲿��");
		for (int i = 0; i < len; i++)
			printf("%f,", imag[i]);
	}
	printf("\n\n");
}


int main()
{

	float32 data[11] = { 3.900000f,-0.913114f,0.732225f,6.333200f,7.778681f,2.574413f,-2.688735f,-1.417364f,3.901729f,5.169487f,-0.100732f};

	//����FFT��Ⱦ��
	CVFFT_RENDER fftcal = YMCV_Creat_FFT_Render_Creat(11, 1, 0);//fft ���ݼ��Ϊ1��float  ��תΪģ���н�
	int fftLen = fftcal->unum;//�õ�fft����ĳ���

	float32* realout = CV_malloc0(fftLen * sizeof(float32));//ʵ��
	float32* imagout = CV_malloc0(fftLen * sizeof(float32));//�鲿
	float32* realresult = CV_malloc0(fftLen * sizeof(float32));
	float32* imagresult = CV_malloc0(fftLen * sizeof(float32));
	

	printComplex(data, NULL, fftcal->danum, "ԭʼ����");
	printf("ԭʼ���ݳ��ȣ�%d��fft�����ȣ�%d\n\n", fftcal->danum, fftcal->unum);
	// FFT �任
	YMCV_FFT_Data_Cal(fftcal, data, NULL, realout, imagout);//ֻ��ʵ��
	printComplex(realout, imagout, fftLen, "FFT�任���");

	//FFT - Shift Ƶ��
    YMCV_FFTShift_Data_Cal(fftcal, realout, imagout, realresult, imagresult);// FFT - Shift
	printComplex(realresult, imagresult, fftLen, "FFT-Shift�任���");

	//������0Ƶ��Ϊ�Գ� ���ҹ���(ʵ����ͬ���鲿�෴)
	int fftLen_2 = fftLen / 2;
	printf("0Ƶ��ǿ��Ϊ:%f\n\n",YMCV_Sqrt(realresult[fftLen_2]* realresult[fftLen_2] + imagresult[fftLen_2] * imagresult[fftLen_2]));

	//FFT - iShift
	YMCV_FFTShift_Data_Cal(fftcal, realresult, imagresult, realout, imagout);// FFT - iShift
	printComplex(realout, imagout, fftLen, "FFT-iShift�任���");

	// IFFT�任
	YMCV_IFFT_Data_Cal(fftcal, realout, imagout, realresult, imagresult);//�����Ϊʵ�����鲿
	printComplex(realresult, imagresult, fftLen, "IFFT�任���");

	//CVFFT_Render ��Ⱦ���ڴ��ͷ�
	YMCV_Free_FFT_RenderFree(fftcal);
}