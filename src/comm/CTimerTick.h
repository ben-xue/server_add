#ifndef CTIMERTICK_H__
#define CTIMERTICK_H__


#include "comm_inc.h"


typedef void (*FnTimerHandle)(void* pUserData);

class CTimerTick
{
public:
	CTimerTick();
	int Init(int iTimerMs, FnTimerHandle fnHandle, void* pUserData);
	void OnTick();

private:
	int m_iTimerMs;	
	timeval m_tLastTv; 
	FnTimerHandle m_fnHandle;
	void* m_pUserData;
};


#endif


