#include "_LUL_Base.h"

#include <filesystem>
#include <fstream>

void _LUL_::AppManager::StartApplication(IApplication* pEntryapp)
{
	L_LOG(Info, L"Starting application...");
	LUL_PROFILER_TIMER_START();

#if _WIN32 || _WIN64
#	ifdef _DEBUG
		AllocConsole();
#	endif // _DEBUG
#endif // !_WIN32

	if (m_pAppInstance != nullptr)
	{
		throw new L_EXCEPT_MSG("An instance of application already exists.");
	}

	// Try to create an instance of application
	m_pAppInstance = pEntryapp;
	try
	{
		m_pAppInstance->Initialize();
	}
	catch (_LUL_::LException e)
	{
		throw e;
	}

	m_bAppInitialized = true;

	// if (_LUL_::AppProps::Get().GetBootTime() == LTIME{ 0 })
	// {
	// 	if (!_LUL_::AppProps::Get().UpdateBootTime())
	// 		throw new L_EXCEPT_MSG("Couldn't update the boot time on application start.");
	// }

	_LUL_::Logger::Get().Start();
}

void _LUL_::AppManager::UpdateApplication()
{
	L_LOG(Warning, L"Application update called on closed application!");

	m_pAppInstance->Update();
}

void _LUL_::AppManager::CloseApplication(LBOOL bExit)
{
	L_LOG(Info, bExit ? L"Exiting application..." : L"Closing application...");
	LUL_PROFILER_TIMER_START();

	// Destroy application
	if (m_pAppInstance)
	{
		if (m_bAppInitialized)
			m_pAppInstance->Destroy();

		m_bAppInitialized = false;
		delete m_pAppInstance;
		m_pAppInstance = nullptr;
	}

	StopLUL();
	
	// Exit check
	if (bExit)
	{
		exit(0);
	}
}

void _LUL_::AppManager::ExitWithErrorMsg(LCHAR const* const sMsg) noexcept
{
	// TO DO: Show error message box etc.

	StopLUL();
	exit(-1);
}

LBOOL _LUL_::AppManager::Create(LWCHAR* sPath)
{
	namespace fs = std::filesystem;
	using namespace std;

	LBOOL result = false;
	LWCHAR* lastSlash = wcsrchr(sPath, '\\');
	if (lastSlash == nullptr)
		lastSlash = wcsrchr(sPath, '/');

	if (*(lastSlash + 1) == L'\0') // If last slash is the last character
	{
		// We are sure that it should be a directory.
		return fs::create_directories(sPath);
	}

	*lastSlash = L'\0'; // Trim the path for the creation of sub path
	fs::create_directories(sPath);
	*lastSlash = L'\\';

	wofstream createFile(sPath, ios_base::binary);
	if (!createFile.is_open())
	{
		result = false;
	}

	createFile.close();
	result = true;
	return result;
}

LBOOL _LUL_::AppManager::CreateInKnownPath(
	LWCHAR* sSubPath, 
	const KnownPaths& kp,
	LWCHAR* sCombinedPath)
{
	LBOOL result = false;
	const LWCHAR* knownPath = _LUL_::AppProps::Get().GetKnownPath(kp);


	return result;
}

// -----------------------------------------------------------------------------
void _LUL_::AppManager::StopLUL()
{
	_LUL_::Logger::Get().Stop();

	// LUL_PROFILER_TIMER_SET_OUTPUT_DIR(_LUL_::AppProps::Get().GetKnownPath(Executable));
	// LUL_PROFILER_TIMER_RESULTS();
#if _WIN32 || _WIN64
#	ifdef _DEBUG
		// TO DO: Wait for input.
		Sleep(2500);
#	endif // _DEBUG
#endif // !_WIN32
}

// _LUL_::IApplication* _LUL_::AppManager::GetAppInstance()
// {
// 	if (!m_pAppInstance)
// 		throw new L_EXCEPT_MSG("Application instance was null on GetAppInstance().");
// 
// 	return m_pAppInstance;
// }
