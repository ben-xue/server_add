#ifndef CAUTO_H__
#define CAUTO_H__

#include "comm_inc.h"

template <class T>
class CAuto
{
public:
    CAuto()
    {
        m_ptr = new T;
    }
    T* GetPtr(){ return m_ptr; }
    ~CAuto()
    {   
        if (m_ptr)
        {
            delete m_ptr;
            m_ptr = NULL;
        }
    }

protected:
    T* m_ptr;
};

#endif


