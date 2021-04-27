//
// Created by jianghz on 2020/10/27.
//

#ifndef KGAME_SERVER_TRUERANDOM_H
#define KGAME_SERVER_TRUERANDOM_H

#include <stdint.h>
#include <stdlib.h>

///// use example
//int64_t DiffTimeMs(const timeval& tv1, const  timeval& tv2)
//{
//    return ((int64_t)tv1.tv_sec-(int64_t)tv2.tv_sec)*1000 +
//           ((int64_t)tv1.tv_usec-(int64_t)tv2.tv_usec)/1000;
//}
//
//
//int main()
//{
//    CTrueRandom random;
//    std::map<int, int> hashMap;
//
//    timeval tvStart, tvEnd;
//    timeval tvMergeStart, tvMergeEnd;
//    gettimeofday(&tvStart, NULL);
//
//    int iLoopCnt = 100000;
//    for (int i = 0; i < iLoopCnt; ++i)
//    {
//        uint32_t iRandom = random.NextUInt32();
////        if (map.find(iRandom) != map.end())
////        {
////            std::cout << "find repeated random " << iRandom << " index " << i << std::endl;
////        }
//    }
//    gettimeofday(&tvEnd, NULL);
//    int64_t iDiffTime = DiffTimeMs(tvEnd, tvStart);
//    std::cout << "iLoopCnt " << iLoopCnt << " use total ms " << iDiffTime << std::endl;
//
//    return 0;
//}

/// test
/// iLoopCnt 100000 use total ms 109

class CTrueRandom
{
public:
    CTrueRandom();
    ~CTrueRandom();

    uint32_t NextUInt32();

    uint32_t NextUInt32(uint32_t max_random);

    bool NextBytes(void* buffer, size_t size);

    uint32_t RandI(uint32_t a, uint32_t b);

    float RandF(float a, float b);

private:
    int32_t m_fd;
};

#endif //KGAME_SERVER_TRUERANDOM_H
