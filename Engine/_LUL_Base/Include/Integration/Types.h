#ifndef LUL_TYPES_H
#define LUL_TYPES_H

#include <ctime>

#ifdef __cplusplus 
	namespace _LUL_
	{
		enum KnownPaths
		{
			LogDirectory = 0x00,
			ExecutableDirectory = 0x01,
	#if _WIN32 || _WIN64
			AppData = 0x02,
			Count = 3
	#elif __GNUC__
			Count = 2
	#else
			Count = 2
	#endif
		};
	}
#endif

#if _WIN32 || _WIN64
typedef bool  LBOOL;
typedef CHAR  LCHAR;
typedef WCHAR LWCHAR;

typedef INT8  LINT8;
typedef INT32 LINT32;
typedef INT64 LINT64;

typedef UINT8  LUINT8;
typedef UINT32 LUINT32;
typedef UINT64 LUINT64;

typedef time_t LTIME;
#elif __GNUC__
typedef bool    LBOOL;
typedef char    LCHAR;
typedef wchar_t LWCHAR;

typedef __int8_t   LINT8;
typedef __int32_t  LINT32;
typedef __int64_t  LINT64;

typedef __uint8_t  LUINT8;
typedef __uint32_t LUINT32;
typedef __uint64_t LUINT64;
#else
#   error "Couldn't detect the system, _LUL_ types are not defined."
#endif

#endif // !LUL_TYPES_H
