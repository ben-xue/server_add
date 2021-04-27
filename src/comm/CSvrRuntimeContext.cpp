#include "CSvrRuntimeContext.h"
#include "CAppFrame.h"

void CSvrRuntimeContext::SetRunningAppFrame(CAppFrame* pFrame)
{
	m_pRunningFrame = pFrame;
}

CAppFrame* CSvrRuntimeContext::GetRunningAppFrame()
{
	return m_pRunningFrame;
}
