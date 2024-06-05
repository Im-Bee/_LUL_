#ifndef _LUL_BASE_H
#define _LUL_BASE_H

#pragma region Environment detection
#define LUL_ENVIRONMENT_INVALID_VALUE (0x00)	// 0
#if _WIN32 || _WIN64
#	if _WIN64
#		define LUL_ENVIRONMENT (0x40) // 64
#	else
#		define LUL_ENVIRONMENT (0x20) // 32
#	endif
#elif __GNUC__
#	if __x86_64__ || __ppc64__
#		define LUL_ENVIRONMENT (0x40) // 64
#	else
#		define LUL_ENVIRONMENT (0x20) // 32
#	endif
#else
#	define LUL_ENVIRONMENT LUL_ENVIRONMENT_INVALID_VALUE
#	error "Couldn't detect the environment, LUL_ENVIRONMENT marco has been set to LUL_ENVIRONMENT_INVALID_VALUE"
#endif
#pragma endregion

#pragma region System specific headers
#if _WIN32 || _WIN64
#	ifndef WIN32_LEAN_AND_MEAN 
#		define WIN32_LEAN_AND_MEAN 
#	endif // !WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#endif // !_WIN32

#include <exception>
#include <thread>
#pragma endregion

#ifdef __cplusplus 
#pragma region Export/Import macro
#if _WIN32 || _WIN64
#	ifdef _LUL_EXPORT
#		define LUL_API __declspec(dllexport)
#	else
#		define LUL_API __declspec(dllimport)
#	endif // !_LUL_EXPORT
#elif __GNUC__
#	define LUL_API 
#	error "_LUL_EXPORT not implemented on current system, LUL_API macro has been left empty"
#else
#	define LUL_API 
#	error "Couln't detect the system, LUL_API macro has been left empty"
#endif
#pragma endregion

#ifdef _DEBUG
#	pragma region System specific debug
#	if _WIN32 || _WIN64
#		define L_CONSOLE_OUT_HELPER(str) (std::wstring(str) + L"\n")
#		define L_CONSOLE_OUT(str) WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), L_CONSOLE_OUT_HELPER(str).c_str(), static_cast<DWORD>(L_CONSOLE_OUT_HELPER(str).size()), NULL, NULL)
#	elif __GNUC__
#		define L_CONSOLE_OUT(str) 
#		error "L_CONSOLE_OUT not implemented on current system, L_CONSOLE_OUT macro has been left empty"
#	else
#		error "Couldn't detect the system, system specific debug not available."
#	endif
#	pragma endregion
#	define L_LOG(...) _LUL_::Logger::Get().Log(__VA_ARGS__)
#else
#	define L_CONSOLE_OUT(str)
#	define L_LOG(...)
#endif // _DEBUG

#include "../../_LUL_Profiler/Include/LUL_Profiler.h"

#include "Integration/Types.h"
#include "Integration/Limits.h"
#include "Debugging/Exceptions.hpp"
#include "Utilities/LVector.hpp"
#include "Utilities/LPair.hpp"
#include "Utilities/LTuple.hpp"
#include "Utilities/LQueue.hpp"
#pragma warning(push)
#pragma warning(disable : 4251)
// Warning	C4251	'_LUL_::Logger::m_tLogger': class 'std::thread' needs to have dll - interface to be used by clients of class '_LUL_::Logger'
#include "Debugging/Logger.hpp"
#pragma warning(pop)
#include "Core/IApplication.hpp"
#include "Core/AppManager.hpp"
#include "Core/AppProperties.hpp"

#else // C --------------------------------------------------------------------
#endif // __cplusplus 
#endif // !_LUL_BASE_H