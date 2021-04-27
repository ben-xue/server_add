/*
**  
*  字符串工具集
*/
#include "CStrUtil.h"

void CStrUtil::SplitStr(const char* szText, const char* szSplit, vector<string> &listSubStr)
{
    const char* ptr = szText;
    for(const char* pSplit = strstr(ptr, szSplit); pSplit != NULL; pSplit = strstr(ptr, szSplit))
    {        
        int iSubLen = pSplit - ptr;
        if (iSubLen > 0)
        {
            string subStr = string(ptr, pSplit-ptr);
            listSubStr.push_back(subStr);
        }

        ptr = pSplit + strlen(szSplit);
    }

    if (ptr[0] != 0)
    {
        listSubStr.push_back(string(ptr));
    }
}

bool CStrUtil::IsNum(const char* s)
{
	while (*s != '\0')
	{
		if (*s < '0' || *s > '9')
		{
			return false;
		}
		s++;
	}
	return true;
}


int CStrUtil::CountSubStr(const char *str, const char *sub)
{
	if (!str || !sub)
	{
		return -1;
	}

	int cnt = 0;
	const char* p = NULL;

	int len = strlen(sub);
	while ((p = strstr(str, sub)) != NULL)
	{
		cnt++;
		str = p + len;
	}

	return cnt;
}

void CStrUtil::ReplaceAllStr(const char *str, const char *sub, const char *replace, char* buff, int buff_len)
{
	if (!str || !sub || !replace || !buff) return; 

	char *pos = (char *)str;
	int count = CountSubStr(str, sub);

	if (0 >= count)
	{
		strncpy(buff, str, buff_len);
		return;
	}
	int size = (
		strlen(str)
		- (strlen(sub) * count)
		+ strlen(replace) * count
		) + 1;

	if (size > buff_len)
	{
		return;
	}

	memset(buff, 0, buff_len);

	char *current;
	while ((current = strstr(pos, sub))) {
		int len = current - pos;
		strncat(buff, pos, len);
		strncat(buff, replace, strlen(replace));
		pos = current + strlen(sub);
	}

	if (pos != (str + strlen(str))) {
//		strncat(buff, pos, (str - pos));
		strncat(buff, pos,strlen(pos));
	}

	return;
}

