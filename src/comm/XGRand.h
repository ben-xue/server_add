#ifndef __FILE_RAND_H__
#define __FILE_RAND_H__

#include <inttypes.h>

uint32_t GetXGRandSeed();

// 返回[a,b]范围内的一个随机整数,如果a>b,返回b
int XGRandI(int a, int b);

// 返回[a,b]范围内的一个随机浮点数,如果a>b,返回b
float XGRandF(float a, float b);

// 给定概率，返回是否命中
bool XGRandHit(int Rate, int Base = 100);

// 给定绝对概率，返回是否命中
bool XGRandHitF(float Rate);

// 给定一组绝对概率，随机返回其中一个
// 概率和一般等于 1，
// 如果概率和大于1，则后面的元素永远选不到
// 如果概率和小于1，则有可能选不到任何元素
// 返回 0~n-1 的数组下标，或 -1 表示未选任何
int XGRandSelectF(float fRate[], int n);

// 给定一组相对权重，随机返回其中一个
int XGRandSelectI(unsigned int iWeight[], int n);
#endif


