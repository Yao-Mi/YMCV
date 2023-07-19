#include"./YMCV_Debug.h"
#include<stdio.h>

#if YMCV_DEBUG_PRINT

#else
//���Դ�����Ϣ���
uint8 CV_Event_LogOutbuff[YMCV_LOGOUT_LEN];

#endif // YMCV_DEBUG_PRINT



/**
  * @brief ���Դ����ӡ
  */
void cv_assert_fail_inform(uint8* failfile, uint32 failline)
{
	cv_log_print("error in file\" %s\" ,line in %d \r\n", failfile, failline);
	while (1) {};
}


/**
  * @brief ���Դ�����ʾ��Ϣ��ӡ
  */
void cv_logout_imform(uint8* mytips, CVEVNLOG event)
{
	switch (event)
	{
	case CV_LOG_PtrI: //����ָ��Ϊ��
	{
		cv_log_print("intput ptr");
		break;
	}
	case CV_LOG_PtrO: //��� ָ��Ϊ��
	{
		cv_log_print("output ptr");
		break;
	}
	case CV_LOG_PtrIO://����&��� ָ��Ϊ��
	{
		cv_log_print("io puts ptr");
		break;
	}
	case CV_LOG_TypeI://���� ���ʹ���
	{
		cv_log_print("intput type");
		break;
	}
	case CV_LOG_TypeO://��� ���ʹ���
	{
		cv_log_print("output type");
		break;
	}
	case CV_LOG_TypeIO://����&��� ���ʹ���
	{
		cv_log_print("io puts type");
		break;
	}
	case CV_LOG_Mem0://С�����ڴ�ʧ��
	{
		cv_log_print("small memory");
		break;
	}
	case CV_LOG_Mem1://�������ڴ�ʧ��
	{
		cv_log_print("big memory");
		break;
	}
	case CV_LOG_ParamI://�����������
	{
		cv_log_print("input param");
		break;
	}
	default:
	{
		cv_log_print("something");
		break;
	}
	}
	cv_log_print(" is error\r\n");
	cv_log_print("Tips:%s\r\n", mytips);
	while(1) {};
}



