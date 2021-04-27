#include "CTimerTick.h"

CTimerTick::CTimerTick()
{
	m_iTimerMs = 0;
	bzero(&m_tLastTv, sizeof(m_tLastTv));
	m_fnHandle = NULL;
	m_pUserData = NULL;
}

int CTimerTick::Init(int iTimerMs, FnTimerHandle fnHandle, void* pUserData)
{
	m_iTimerMs = iTimerMs;
	m_fnHandle = fnHandle;
	m_pUserData = pUserData;
	m_tLastTv = CTime::GetTimeVal();
	return 0;
}

void CTimerTick::OnTick()
{
	ASSERT(m_fnHandle && m_iTimerMs > 0);
	
	timeval tvNow = CTime::GetTimeVal();
	int64_t diffMs = CTime::DiffTimeMs(tvNow, m_tLastTv);
	if (diffMs >= m_iTimerMs)
	{
		m_tLastTv = CTime::GetTimeVal();
		m_fnHandle(m_pUserData);
	}
}
	

