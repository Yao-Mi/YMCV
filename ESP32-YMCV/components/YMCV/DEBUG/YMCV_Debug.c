#include "./YMCV_Debug.h"
#include <stdio.h>

#if YMCV_DEBUG_PRINT

#else
// 调试错误信息输出
uint8 CV_Event_LogOutbuff[YMCV_LOGOUT_LEN];

#endif // YMCV_DEBUG_PRINT

/**
 * @brief 断言错误打印
 */
void cv_assert_fail_inform(uint8 *failfile, uint32 failline)
{
	cv_log_print("error in file\" %s\" ,line in %d \r\n", failfile, failline);
	while (1)
	{
	};
}

/**
 * @brief 调试错误提示信息打印
 */
void cv_logout_imform(uint8 *mytips, CVEVNLOG event)
{
	switch (event)
	{
	case CV_LOG_PtrI: // 输入指针为空
	{
		cv_log_print("intput ptr");
		break;
	}
	case CV_LOG_PtrO: // 输出 指针为空
	{
		cv_log_print("output ptr");
		break;
	}
	case CV_LOG_PtrIO: // 输入&输出 指针为空
	{
		cv_log_print("io puts ptr");
		break;
	}
	case CV_LOG_TypeI: // 输入 类型错误
	{
		cv_log_print("intput type");
		break;
	}
	case CV_LOG_TypeO: // 输出 类型错误
	{
		cv_log_print("output type");
		break;
	}
	case CV_LOG_TypeIO: // 输入&输出 类型错误
	{
		cv_log_print("io puts type");
		break;
	}
	case CV_LOG_Mem0: // 小申请内存失败
	{
		cv_log_print("small memory");
		break;
	}
	case CV_LOG_Mem1: // 大申请内存失败
	{
		cv_log_print("big memory");
		break;
	}
	case CV_LOG_ParamI: // 输入参数错误
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
	while (1)
	{
	};
}
