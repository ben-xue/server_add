/*
**  
*  通用的服务器上下文状态
*/

#ifndef CSVR_RUNTIME_CONTEXT_H__
#define CSVR_RUNTIME_CONTEXT_H__

#include "comm_inc.h"
#include "CSingleton.h"
#include "CAppFrame.h"

class CAppFrame;

class CSvrRuntimeContext : public CSingleton<CSvrRuntimeContext>
{
public:	
	void SetRunningAppFrame(CAppFrame* pFrame);
	
	CAppFrame* GetRunningAppFrame();
	 
private:
	CAppFrame* m_pRunningFrame;

};

#endif


