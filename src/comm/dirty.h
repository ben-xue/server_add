#ifndef XGDIRTY_H__
#define XGDIRTY_H__

class CPlayer;

enum Dirty_Ckeck_Type
{
	DIRTY_TYPE_NULL = 0x00,
	DIRTY_TYPE_CHAT = 0x01,
	DIRTY_TYPE_NAME = 0x02,
};

int GetUtfCharLen(unsigned char firstChar);

int CheckNameUtfRule(const char *pszStr);

void TrimStr(char * pszString);

bool CheckEmoji(const char *pszStr);

#endif

