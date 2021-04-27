#include "comm/tinyxml2.h"
#include "XGXmlUtil.h"


using namespace tinyxml2;

int XGXmlUtil::GetInt(const XMLElement* pElem, const char* szItemName)
{
	const XMLElement* pSubElem = pElem->FirstChildElement(szItemName);
	if (pSubElem)
	{
		int iText = 0;
		if (XML_SUCCESS == pSubElem->QueryIntText(&iText))
		{
			return iText;
		}
	}

	return 0;
}

float XGXmlUtil::GetFloat(const XMLElement* pElem, const char* szItemName)
{
	const XMLElement* pSubElem = pElem->FirstChildElement(szItemName);
	if (pSubElem)
	{
		float fText = 0;
		if (XML_SUCCESS == pSubElem->QueryFloatText(&fText))
		{
			return fText;
		}
	}
	
	return 0.0f;
}

const char* XGXmlUtil::GetText(const XMLElement* pElem, const char* szItemName)
{
	const XMLElement* pSubElem = pElem->FirstChildElement(szItemName);
	if (pSubElem)
	{
		return pSubElem->GetText();
	}
	
	return NULL;
}


