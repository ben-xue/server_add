#ifndef XGXMLUTIL_H__
#define XGXMLUTIL_H__

#include "comm/tinyxml2.h"

using namespace tinyxml2;

struct tagCSMapVector3;

class XGXmlUtil
{
public:
	static int GetInt(const XMLElement* pElem, const char* szItemName);
	static float GetFloat(const XMLElement* pElem, const char* szItemName);
	static const char* GetText(const XMLElement* pElem, const char* szItemName);
};

#endif
