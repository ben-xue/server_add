
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dirty.h"
#include "pal/pal_os.h"

void TrimStr(char * pszString)
{
	char * pb;
    char * pe;
    size_t iTempLength;
	
    if(pszString == NULL)
    {
        return;
    }

    iTempLength = strlen(pszString);
    if(iTempLength == 0)
    {
        return;
    }

    pb = pszString;
    while ( ( (*pb == ' ') || (*pb == '\t') || (*pb == '\n') || (*pb == '\r') ) && (*pb != 0))
    {
        pb++;
    }

    pe = &pszString[iTempLength - 1];
    while ( (pe >= pb) && ( (*pe == ' ') || (*pe == '\t') || (*pe == '\n') || (*pe == '\r') ) )
    {
        pe--;
    }
    *(pe + 1) = '\0';

    strcpy( pszString, pb );

    return;
}

int GetUtfCharLen(unsigned char firstChar)
{
	int iLen = 0;
	for (int i = 0; i < 4; i++)
	{
		if ((firstChar & ( 1 << (7-i))) != 0)
		{
			iLen++;
		}
		else
		{
			break;
		}
	}
	
	return iLen;
}

int CheckNameUtfRule (const char *pszStr)
{
	bool bHasEmoji = CheckEmoji(pszStr);
	if (bHasEmoji)
	{
		return -1;
	}

    const unsigned char *p ;
	int iUtfCharLen = 0;
    for ( p = (const unsigned char *)pszStr ; *p ; p ++ )
    {
		/**
		*检查是否是正常的Utf8编码
		UTF-8的编码规则很简单，只有二条：
			1）对于单字节的符号，字节的第一位设为0，后面7位为这个符号的unicode码。因此对于英语字母，UTF-8编码和ASCII码是相同的；
			2）对于n字节的符号，第一个字节的前n位都设为1，第n+1位设为0，后面字节的前两位一律设为10。剩下的二进制位，表示为这个符号的unicode码；
		*/
		
    	if (iUtfCharLen <= 0)
		{
			iUtfCharLen = GetUtfCharLen(*p);
			if (iUtfCharLen <= 0)
			{
				if ( ( *p >= '0' &&  *p <='9'  ) || (*p >= 'a' && *p <= 'z' ) || ( *p >= 'A' && *p <= 'Z' ) || ( *p == '_' ) )
				{
					continue ;
				}

				return -1;
			}
			else
			{
				iUtfCharLen--;
			}
		}
		else
		{
			//如果是utf8后续的字节
			iUtfCharLen--;
			if ( (unsigned char)*p >= 0x80 )
			{
				continue;
			}

			return -1;
		}
    }

    return 0 ;
}

bool CheckEmoji(const char *pszStr)
{
	//https://apps.timwhitlock.info/emoji/tables/unicode#block-4-enclosed-characters
	bool bFind = false;
	int iLen = strlen(pszStr);
	if (iLen <= 0)
	{
		return bFind;
	}

	//不判断倒数一个字符
	//emoji编码长度不一，我们只判断前两个字符
	iLen = iLen - 1;

	for (int i = 0; i < iLen; i++)
	{
		uint8_t ch = pszStr[i];
		uint8_t ch2 = pszStr[i + 1];

		if (uint8_t(0xf0) == ch)
		{
			if (uint8_t(0x90) <= ch2 && ch2 <= uint8_t(0x9f))
			{
				bFind = true;
				break;
			}
		}
		else if (uint8_t(0xc2) == ch)
		{
			if (uint8_t(0xa9) <= ch2 && ch2 <= uint8_t(0xae))
			{
				bFind = true;
				break;
			}
		}
		else if (uint8_t(0xe2) == ch)
		{
			if (uint8_t(0x80) <= ch2 && ch2 <= uint8_t(0x9f))
			{
				bFind = true;
				break;
			}

			if (uint8_t(0xa1) <= ch2 && ch2 <= uint8_t(0xaf))
			{
				bFind = true;
				break;
			}
		}
		else if (uint8_t(0xee) == ch)
		{
			//like ee9092 
			if (uint8_t(0x90) <= ch2 && ch2 <= uint8_t(0x9f))
			{
				bFind = true;
				break;
			}
		}
	}

	return bFind;
}