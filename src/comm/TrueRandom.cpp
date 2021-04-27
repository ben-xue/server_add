//
// Created by jianghz on 2020/10/27.
//

#include <fcntl.h>
#include <unistd.h>

#include "TrueRandom.h"

CTrueRandom::CTrueRandom()
    : m_fd(-1)
{
    m_fd = open("/dev/urandom", O_RDONLY, 0);
    if (m_fd < 0)
    {
        //
        abort();
    }
}

CTrueRandom::~CTrueRandom()
{
    close(m_fd);
    m_fd = -1;
}

uint32_t CTrueRandom::NextUInt32()
{
    uint32_t random = -1;
    NextBytes(&random, sizeof(random));
    return random;
}

uint32_t CTrueRandom::NextUInt32(uint32_t max_random)
{
    return NextUInt32() % max_random;
}

bool CTrueRandom::NextBytes(void *buffer, size_t size)
{
    return read(m_fd, buffer, size) == static_cast<int32_t>(size);
}

uint32_t CTrueRandom::RandI(uint32_t a, uint32_t b)
{
    if(a==b)
    {
        return a;
    }
    else if(a>b)
    {
        return b;
    }
    else
    {
        return a + static_cast<uint32_t>((b-a+1)*(NextUInt32(RAND_MAX)/(RAND_MAX+1.0)));
    }
}

float CTrueRandom::RandF(float a, float b)
{
    if(a==b)
    {
        return a;
    }
    else if(a>b)
    {
        return b;
    }
    else
    {
        if(a<0)
        {
            return a+RandF(0, b-a);
        }
        else
        {
            return a+((b-a)*(NextUInt32(RAND_MAX)/(RAND_MAX+0.0)));
        }
    }
}
