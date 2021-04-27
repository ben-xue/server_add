#ifndef DSF_STD_INC_H__
#define DSF_STD_INC_H__

/*
 * @file: stdinc.h
 * @author: pls
 * @brief: C库文件包含 及 若干宏定义
 *
*/

//------------------compatible c part--------------
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <ctime>
#include <cstdint>
#include <climits>
#include <cstddef>
#include <csetjmp>
#include <cctype>
#include <csignal>
#include <clocale>
#include <cassert>
#include <cerrno>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

//常用stl数据结构

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <set>
using std::set;
#include <algorithm>
#include <list>
#include <stack>
#include <iterator>
#include <bitset>
#include <stack>
#include <queue>

/////////////////////////std=c++11/////////////////
#include <memory>
#include <functional>
#include <random>
#include <array>
#include <bitset>
#include <sstream>
#include <iostream>
#include <fstream>

//--------------------c++11 thread-----------------
#include <thread>
#include <mutex> 
#include <condition_variable>
//---------------c++11 code converting ------------
#include <regex>


#ifndef _WIN32

//------------------linux system call -------------
//-------------------------------------------------
#ifndef gettid
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)
#endif

#include <unistd.h>
#include <dirent.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/file.h>
#include <sys/syscall.h>

#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/un.h>
#include <sys/mman.h>
#include <netinet/tcp.h>

//thread
#include <pthread.h>
#include <semaphore.h>

#endif


//////////////////////////////////////////////////////
#ifndef restrict
#define restrict 
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#if !defined(UNUSED)
#define UNUSED(x)	((void)(x))
#endif

///////////////////////////////////////////////////////
#ifndef NS_BEGIN
#define NS_BEGIN(ns)	namespace ns {
#define NS_END()		};
#endif


#endif