#include "_LUL_Base.h"

// Logger ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void _LUL_::Logger::Start() noexcept
{
	if (!CreateTargetDir())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::CreateTargetDir() noexcept failed.");
	}
	if (!CreateOutTarget())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::CreateOutTarget() noexcept failed.");
	}
	if (!CleanOldData())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::CleanOldData() noexcept failed.");
	}
	if (!StartThread())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::StartThread() noexcept failed.");
	}
}

// -----------------------------------------------------------------------------
void _LUL_::Logger::Log(Severity s, const LWCHAR* format, ...)
{
}

// -----------------------------------------------------------------------------
void _LUL_::Logger::Stop() noexcept
{
	if (!StopThread())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::StopThread() noexcept failed.");
	}
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
LBOOL _LUL_::Logger::CreateTargetDir() noexcept
{
	return LBOOL();
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::Logger::CreateOutTarget() noexcept
{
	return LBOOL();
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::Logger::CleanOldData() noexcept
{
	return LBOOL();
}

// -----------------------------------------------------------------------------
void _LUL_::Logger::Output()
{
	L_CONSOLE_OUT(L"TO DO: Here, goes output");
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::Logger::StartThread() noexcept
{
	try
	{
		m_tLogger = new std::thread(&_LUL_::Logger::SeparateThreadLoop, this);
	}
	catch (...)
	{
		return false;
	}

	m_bThreadStatus = true;
	return true;
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::Logger::StopThread() noexcept
{
	if (!m_bThreadStatus &&
		m_tLogger == nullptr)
		return true;

	if (m_tLogger != nullptr &&
		m_tLogger->joinable())
	{
		m_bThreadStatus.store(false);
		m_tLogger->join();
		delete m_tLogger;
		m_tLogger = nullptr;
		return true;
	}

	return false;
}

// -----------------------------------------------------------------------------
void _LUL_::Logger::SeparateThreadLoop()
{
	while (m_bThreadStatus.load())
	{

	}
}
