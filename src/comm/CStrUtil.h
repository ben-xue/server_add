/*
**  
*  字符串管理接口
*/

#ifndef CSTRUTIL_H__
#define CSTRUTIL_H__

#include <sys/time.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;


class CStrUtil
{
public:
	static void SplitStr(const char* szText, const char* szSplit, vector<string> &listSubStr);
	template <typename T>
	static string ToStr(const T&);

	static bool IsNum(const char* s);

	static void ReplaceAllStr(const char *str, const char *sub, const char *replace, char* buff, int buff_len);

	static int CountSubStr(const char *str, const char *sub);
};

template <typename T>
string CStrUtil::ToStr(const T& value)
{
	std::ostringstream out;
	out << value;
	return out.str();
}

#endif


