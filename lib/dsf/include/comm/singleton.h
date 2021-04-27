#pragma once

#include "stdinc.h"

NS_BEGIN(dsf)

template <class T>
class CSingleton
{
public:	
	static T* Instance()
	{
		static T s_instance;
		return &s_instance;
	}
};


NS_END()