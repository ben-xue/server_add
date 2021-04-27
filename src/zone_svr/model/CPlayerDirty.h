//
// Created by Administrator on 2021/4/26.
//

#ifndef SERVER_CPLAYERDIRTY_H
#define SERVER_CPLAYERDIRTY_H


class CPlayerDirty
{
public:
	virtual bool IsDirty() { return m_bDirty; }

	virtual void MarkDirty() { m_bDirty = true; }

	virtual void ClearDirty() { m_bDirty = false; }

	//返回true表示已经不再脏了，返回false表示还脏
	virtual bool OnDirty() 
	{
		return true;
	}

private:
	bool m_bDirty;
};


#endif //SERVER_CPLAYERDIRTY_H
