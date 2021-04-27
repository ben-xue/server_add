#ifndef CSINGLETON_H__
#define CSINGLETON_H__

#include "comm_inc.h"

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

#endif


