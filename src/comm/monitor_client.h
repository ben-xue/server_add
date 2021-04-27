#ifndef __MONITOR_API_H__
#define __MONITOR_API_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
	//初始化API
	//	 FileName: mmap循环队列文件
	int Monitor_Init(const char* FileName);
	
	
	// 累加上报API
	//	ServiceName: 服务名
	//	AttrName: 属性名
	//	Value: 累加上报的上报值
	int Monitor_Add(const char* ServiceName, const char* AttrName, uint32_t Value);
	
	// 覆盖上报API，每分钟只取第一次覆盖上报的数据
	//	ServiceName: 服务名
	//	AttrName: 属性名
	//	Value: 覆盖上报的上报值
	// 	特别注意:业务对于同一个上报属性请自行保证只采用一种上报方式，
	//                             切换Add/Set上报同一个属性可能会导致统计异常
	int Monitor_Set(const char* ServiceName, const char* AttrName, uint32_t Value);
#ifdef __cplusplus
}
#endif

#endif
