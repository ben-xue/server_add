#include "XGRand.h"
#include <stdlib.h>

static uint32_t XGRAND_SEED = 0;

uint32_t GetXGRandSeed()
{
    return XGRAND_SEED;
}

int XGRandI(int a, int b)
{
    XGRAND_SEED++;
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
            return a+XGRandI(0, b-a);
        }
        else
        {
            return a+(int)((b-a+1)*(rand()/(RAND_MAX+1.0)));
        }
    }
}

float XGRandF(float a, float b)
{
    XGRAND_SEED++;
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
            return a+XGRandF(0, b-a);
        }
        else
        {
            return a+((b-a)*(rand()/(RAND_MAX+0.0)));
        }
    }
}

bool XGRandHit(int Rate, int Base /*= 100*/)
{
	int Val = XGRandI(0, Base);
	if (Val <= Rate)
	{
		return true;
	}
	return false;
}

bool XGRandHitF(float Rate)
{
	float Val = XGRandF(0, 1);
	if (Val <= Rate)
	{
		return true;
	}
	return false;
}

int XGRandSelectF(float fRate[], int n)
{
	int index = 0;
	float fRand = XGRandF(0, 1);
	float fSum = 0;

	while (index < n && fRand > fSum)
	{
		fSum += fRate[index++];
	}

	if (fRand > fSum)
	{
		return -1;
	}
	else
	{
		return index - 1;
	}
}

int XGRandSelectI(unsigned int iWeight[], int n)
{
	int i = 0;
	unsigned int iSum = 0;
	while (i < n) iSum += iWeight[i++];

	int index = 0;
	int iRand = XGRandI(0, iSum);
	unsigned int iwSum = 0;

	while (index < n && iRand > iwSum)
	{
		iwSum += iWeight[index++];
	}

	if (iRand > iwSum)
	{
		return -1;
	}
	else
	{
		return index - 1;
	}
}
