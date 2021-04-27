
#ifndef KGAME_SERVER_CMSGQUEUE_H
#define KGAME_SERVER_CMSGQUEUE_H


//线程间消息队列


struct stMsgQueueHead
{
    volatile unsigned int size;
    volatile unsigned int in;
    volatile unsigned int out;
};

class CMsgQueue
{
public:
    int Init(int iShmKey, int dwShmSize);

    int Push(char* pMsg, unsigned int iLen);

    int Pop(char* pMsg, unsigned int& iLen);

    bool Empty();

private:
    stMsgQueueHead* m_pQueueHead;
    char* m_pBuffer;
    char* m_pShmMem;
};

#endif //KGAME_SERVER_CMSGQUEUE_H
