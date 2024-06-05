#ifndef LUL_LIMITS_H
#define LUL_LIMITS_H

#define LUL_MAX_ERROR_MSG		static_cast<LUINT8>(0xFF)  // 255
#define LUL_MAX_PATH_SIZE		static_cast<LUINT8>(0xFF)  // 255
#define LUL_MAX_LOG_SIZE		static_cast<LUINT8>(0x400) // 1024

#define LUL_AMOUNT_OF_KNOWN_PATHS static_cast<LUINT8>(_LUL_::KnownPaths::Count) 

#endif // !LUL_LIMITS_H