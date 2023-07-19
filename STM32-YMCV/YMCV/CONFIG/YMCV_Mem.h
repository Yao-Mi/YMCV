#ifndef YMCV_Mem_H
#define YMCV_Mem_H

#include "../CONFIG/YMCV_PubType.h"

void * CV_malloc0(size_t size);//С���ݻ��������룬�ڽϸ��ٶȵĻ���������
void* CV_realloc0(void* ptr, size_t size);//�����ڴ泤��
void CV_free0(void * ptr);//�ͷ��ڴ�

void * CV_malloc1(size_t size);//�����ݻ��������룬�ڽϵ��ٶȵĻ���������
void* CV_realloc1(void* ptr, size_t size);//�����ڴ泤��
void CV_free1(void * ptr);//�ͷ��ڴ�

void CV_memset(void* ptr, int val, size_t size);//�ڴ�ֵ����
void CV_memcpy(void* dst, const void* src, size_t size);//�ڴ濽��

#endif
