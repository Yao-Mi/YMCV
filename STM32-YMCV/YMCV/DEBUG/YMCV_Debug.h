#ifndef _YMCV_DEBUG_H
#define _YMCV_DEBUG_H

#include"../../CONFIG/Pubtype.h"

#define YMCV_ASSERT_DEBUG 0 //断言调试
#define YMCV_DEBUG_MODE   1 //调试模式
#define YMCV_DEBUG_PRINT  1 //直接打印

//调试信息打印输出
#if YMCV_DEBUG_PRINT
#include <stdio.h>
#define cv_log_print(...)  printf(__VA_ARGS__)
#else 

#define cv_log_print(...) //取消输出

#define YMCV_LOGOUT_LEN 100 //调试信息输出缓冲区
extern uint8 CV_Event_LogOutbuff[YMCV_LOGOUT_LEN];

#endif // YMCV_DEBUG_PRINT



//调试的日志信息类型
typedef enum
{
	CV_LOG_OK = 0x00,//运行正常
	CV_LOG_PtrI   = 0x01, //输入指针为空
	CV_LOG_PtrO   = 0x02, //输出 指针为空
	CV_LOG_PtrIO  = 0x03, //输入&输出 指针为空  CV_LOG_PtrI|CV_LOG_PtrO =1+2=3

	CV_LOG_TypeI  = 0x04, //输入 类型错误
	CV_LOG_TypeO  = 0x08, //输出 类型错误
	CV_LOG_TypeIO = 0x0C, //输入&输出 类型错误  CV_LOG_TypeI|CV_LOG_TypeO=4+8=c
	CV_LOG_Mem0   = 0x10, //小申请内存失败
	CV_LOG_Mem1   = 0x20, //大申请内存失败

	CV_LOG_ParamI = 0x40,//输入数据错误
}CVEVNLOG;



#if YMCV_ASSERT_DEBUG
//定义断言
#define cv_assert(x)  ((x)? (void)0U:  cv_assert_fail_inform((uint8*)__FILE__,__LINE__))
//断言信息输出
void cv_assert_fail_inform(uint8* failfile, uint32 failline);

#else
#define cv_assert(x)
#endif // YMCV_ASSERT_DEBUG


#if YMCV_DEBUG_MODE

//解释
#define cv_log_explain(logthis,logtype,logtips) ((logthis)? cv_logout_imform((uint8*)logtips,logtype):(void)0U)
//提示信息
void cv_logout_imform(uint8* tips,CVEVNLOG event);
#else
#define cv_log_explain(logthis,logtype,logtips)
#endif



#endif
