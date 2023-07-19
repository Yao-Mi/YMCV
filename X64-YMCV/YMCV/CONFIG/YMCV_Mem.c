#include <stdlib.h>
#include <string.h>
#include "./YMCV_Mem.h"
//С�����������
//�ڽϸ��ٶȵĻ���������
void * CV_malloc0(size_t size)
{
	void* myptr = malloc(size);

	return myptr;
}
//���·����ڴ泤��
void* CV_realloc0(void* ptr, size_t size)
{
	void* myptr = realloc(ptr, size);

	return myptr;
}
//�ͷ��ڴ�
void CV_free0(void * ptr)
{
	free(ptr);
}

//�������������
//�ڽϵ��ٶȵĻ���������
void * CV_malloc1(size_t size)
{
	void* myptr = malloc(size);

	return myptr;
}

//���·����ڴ泤��
void* CV_realloc1(void* ptr ,size_t size)
{
	void* myptr = realloc(ptr,size);

	return myptr;
}

//�ͷ��ڴ�
void CV_free1(void * ptr)
{
	free(ptr);
}

//�ڴ�ֵ����
void CV_memset(void* ptr, int val , size_t size)
{
	memset(ptr, val, size);
}

//�ڴ�ֵ����
void CV_memcpy(void* dst, void* src, size_t size)
{
	memcpy(dst, src, size);
}
