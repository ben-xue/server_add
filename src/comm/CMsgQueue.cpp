#include "CMsgQueue.h"
#include <algorithm>
#include <cstdlib>
#include "klog.h"

using namespace std;
using namespace dsf;

int CMsgQueue::Init(int iShmKey, int dwShmSize)
{
    bool bResume = false;
    int iShmId = shmget(iShmKey, dwShmSize, 0660 | IPC_CREAT | IPC_EXCL);
    if (iShmId < 0)
    {
        if (errno == EEXIST)
        {
            iShmId = shmget(iShmKey, dwShmSize, 0660);
            bResume = true;
        }
    }

    if (iShmId < 0)
    {
        LOG_ERR("init shm failed shmkey[%d]", iShmKey);
        return -1;
    }

    char *m_pShmMem = (char*)shmat(iShmId, 0, 0);
    m_pBuffer = NULL;
    m_pQueueHead = (stMsgQueueHead*)m_pShmMem;

    if (!bResume)
    {
        m_pQueueHead->size = dwShmSize - sizeof(stMsgQueueHead) - sizeof(int);
    }
    else
    {
        if (m_pQueueHead->size != dwShmSize - sizeof(stMsgQueueHead)-sizeof(int))
        {
            LOG_ERR("resume queue size[%d] not equal compute", m_pQueueHead->size);
            return -1;
        }
    }

    m_pBuffer = m_pShmMem + sizeof(stMsgQueueHead);

    return 0;
}

int CMsgQueue::Push(char* pMsg, unsigned int iLen)
{
    if (m_pQueueHead->size - m_pQueueHead->in + m_pQueueHead->out <= iLen + sizeof(int))
    {
        return -1;
    }

    __sync_synchronize();

    int iTailSize = m_pQueueHead->size - (m_pQueueHead->in % m_pQueueHead->size);
    memcpy(m_pBuffer + m_pQueueHead->in % m_pQueueHead->size, (char*)&iLen, sizeof(int));

	unsigned int msg_in = m_pQueueHead->in + std::min(iTailSize, (int)sizeof(int));

    int l = min(iLen, m_pQueueHead->size - (msg_in % m_pQueueHead->size));

    memcpy(m_pBuffer + msg_in % m_pQueueHead->size, pMsg, l);
    memcpy(m_pBuffer, pMsg + l, iLen - l);

    __sync_synchronize();

    m_pQueueHead->in += (iLen+std::min(iTailSize, (int)sizeof(int)));

    return 0;
}

int CMsgQueue::Pop(char* pMsg, unsigned int& iLen)
{
    if (m_pQueueHead->in == m_pQueueHead->out)
    {
        iLen = 0;

        return 0;
    }

    unsigned int msglen = 0;

    memcpy((char*)&msglen, m_pBuffer + m_pQueueHead->out % m_pQueueHead->size, sizeof(int));
    if (iLen < msglen)
    {
        return -1;
    }

    __sync_synchronize();

    int iTailSize = m_pQueueHead->size - (m_pQueueHead->out % m_pQueueHead->size);
	unsigned int msg_out = m_pQueueHead->out + std::min(iTailSize, (int)sizeof(int));
	
    int l = min(msglen, m_pQueueHead->size - (msg_out % m_pQueueHead->size));
    memcpy(pMsg, m_pBuffer + msg_out % m_pQueueHead->size, l);
    memcpy(pMsg + l, m_pBuffer, msglen - l);

    __sync_synchronize();

    iLen = msglen;
    m_pQueueHead->out += (msglen + std::min(iTailSize, (int)sizeof(int)));

    return 0;
}

bool CMsgQueue::Empty()
{
    return m_pQueueHead->in == m_pQueueHead->out;
}
