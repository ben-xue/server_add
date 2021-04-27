
#ifndef PAL_OS_H__
#define PAL_OS_H__

#include <stdlib.h>

#ifdef _WIN32
	#if defined (LITTLEENDIAN) && LITTLEENDIAN >0 
		#define OS_LITTLEENDIAN

		#if defined (OS_BIGENDIAN)
			#undef OS_BIGENDIAN
		#endif
	#else
		#define OS_BIGENDIAN
		#if defined (OS_LITTLEENDIAN)
			#undef OS_LITTLEENDIAN
		#endif
	#endif

	#define OS_PATH_SEP '\\'

	///todo... win32下要加上结尾设为0
#define OS_SNPRINTF	_snprintf
#define OS_VSNPRINTF _vsnprintf_s

#include <winsock.h>

#define bzero(s, n) memset(s, 0, n)


#else /* WIN32 */

	#if __BYTE_ORDER == __LITTLE_ENDIAN
		#define OS_LITTLEENDIAN

		#if defined (OS_BIGENDIAN)
			#undef OS_BIGENDIAN
		#endif
	#else
		#define OS_BIGENDIAN
		#if defined (OS_LITTLEENDIAN)
			#undef OS_LITTLEENDIAN
		#endif
	#endif
	
	#define OS_PATH_SEP '/'
	#define OS_SNPRINTF snprintf

#include <arpa/inet.h>

#endif /* WIN32 */

#ifdef _WIN32
	#define	OS_DIRSEP				'\\'
#else /* WIN32 */
	#define	OS_DIRSEP				'/'
#endif /* WIN32 */


#define OS_STRNCPY(dst,src,n) do{strncpy(dst,src,n-1);dst[n-1]=0;}while(0)
#define STRNCPY(dst,src,n) do{strncpy(dst,src,n-1);dst[n-1]=0;}while(0)


#ifdef __cplusplus
extern "C"
{
#endif

/* PS Method End. */


/* PA Method Start. */

#define tos_swap64(x) \
     ((((x) & 0xff00000000000000ull) >> 56)                                   \
      | (((x) & 0x00ff000000000000ull) >> 40)                                 \
      | (((x) & 0x0000ff0000000000ull) >> 24)                                 \
      | (((x) & 0x000000ff00000000ull) >> 8)                                  \
      | (((x) & 0x00000000ff000000ull) << 8)                                  \
      | (((x) & 0x0000000000ff0000ull) << 24)                                 \
      | (((x) & 0x000000000000ff00ull) << 40)                                 \
      | (((x) & 0x00000000000000ffull) << 56))

#ifdef OS_LITTLEENDIAN
#define ntohq(x)                    tos_swap64(x)
#define htonq(x)                    tos_swap64(x)
#else
#define ntohq(x)                    (x)
#define htonq(x)                    (x)
#endif

#define NTOHNS(d, s, size)			{			  \
	int i;								      	  \
	for(i=0; i<size; i++)					      \
	{								      		  \
		*(unsigned short*)d	=	ntohs(*(unsigned short*)s);  \
		d	+=	sizeof(unsigned short);			  \
		s	+=	sizeof(unsigned short);			  \
	}								      		  \
}

#define HTONNS(d, s, size)			{			  \
	int i;								          \
	for(i=0; i<size; i++)						  \
	{								      		  \
		*(unsigned short*)d	=	htons(*(unsigned short*)s); \
		d	+=	sizeof(unsigned short);			  \
		s	+=	sizeof(unsigned short);		 	  \
	}								      		  \
}

#define NTOHNL(d, s, size)			{			  \
	int i;								      	  \
	for(i=0; i<size; i++)						  \
	{								              \
		*(unsigned int*)d	=	ntohl(*(unsigned int*)s);  \
		d	+=	sizeof(unsigned int);			  \
		s	+=	sizeof(unsigned int);			  \
	}								      		  \
}

#define HTONNL(d, s, size)			{			  \
	int i;								      	  \
	for(i=0; i<size; i++)						  \
	{								      		  \
		*(unsigned int*)d	=	htonl(*(unsigned int*)s); \
		d	+=	sizeof(unsigned int);			  \
		s	+=	sizeof(unsigned int);			  \
	}								      		  \
}

#define NTOHNQ(d, s, size)			{			  \
	int i;								      	  \
	for(i=0; i<size; i++)						  \
	{								      		  \
		*(ulonglong*)d	=	ntohq(*(ulonglong*)s);	 \
		d	+=	sizeof(ulonglong);			      \
		s	+=	sizeof(ulonglong);			      \
	}								      		  \
}

#define HTONNQ(d, s, size)			{			   \
	int i;								           \
	for(i=0; i<size; i++)						   \
	{								      		   \
		*(ulonglong*)d	=	htonq(*(ulonglong*)s); \
		d	+=	sizeof(ulonglong);			       \
		s	+=	sizeof(ulonglong);			       \
	}								      	       \
}



#include <stdint.h>

typedef uint32_t DR_TIME;
typedef uint32_t DR_DATE;
typedef uint64_t DR_DATETIME;


#ifdef __cplusplus
}
#endif


#endif /* PAL_OS_H__ */

