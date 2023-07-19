#ifndef _YMCV_DEBUG_H
#define _YMCV_DEBUG_H

#include"../../CONFIG/Pubtype.h"

#define YMCV_ASSERT_DEBUG 0 //���Ե���
#define YMCV_DEBUG_MODE   1 //����ģʽ
#define YMCV_DEBUG_PRINT  1 //ֱ�Ӵ�ӡ

//������Ϣ��ӡ���
#if YMCV_DEBUG_PRINT
#include <stdio.h>
#define cv_log_print(...)  printf(__VA_ARGS__)
#else 

#define cv_log_print(...) //ȡ�����

#define YMCV_LOGOUT_LEN 100 //������Ϣ���������
extern uint8 CV_Event_LogOutbuff[YMCV_LOGOUT_LEN];

#endif // YMCV_DEBUG_PRINT



//���Ե���־��Ϣ����
typedef enum
{
	CV_LOG_OK = 0x00,//��������
	CV_LOG_PtrI   = 0x01, //����ָ��Ϊ��
	CV_LOG_PtrO   = 0x02, //��� ָ��Ϊ��
	CV_LOG_PtrIO  = 0x03, //����&��� ָ��Ϊ��  CV_LOG_PtrI|CV_LOG_PtrO =1+2=3

	CV_LOG_TypeI  = 0x04, //���� ���ʹ���
	CV_LOG_TypeO  = 0x08, //��� ���ʹ���
	CV_LOG_TypeIO = 0x0C, //����&��� ���ʹ���  CV_LOG_TypeI|CV_LOG_TypeO=4+8=c
	CV_LOG_Mem0   = 0x10, //С�����ڴ�ʧ��
	CV_LOG_Mem1   = 0x20, //�������ڴ�ʧ��

	CV_LOG_ParamI = 0x40,//�������ݴ���
}CVEVNLOG;



#if YMCV_ASSERT_DEBUG
//�������
#define cv_assert(x)  ((x)? (void)0U:  cv_assert_fail_inform((uint8*)__FILE__,__LINE__))
//������Ϣ���
void cv_assert_fail_inform(uint8* failfile, uint32 failline);

#else
#define cv_assert(x)
#endif // YMCV_ASSERT_DEBUG


#if YMCV_DEBUG_MODE

//����
#define cv_log_explain(logthis,logtype,logtips) ((logthis)? cv_logout_imform((uint8*)logtips,logtype):(void)0U)
//��ʾ��Ϣ
void cv_logout_imform(uint8* tips,CVEVNLOG event);
#else
#define cv_log_explain(logthis,logtype,logtips)
#endif



#endif
