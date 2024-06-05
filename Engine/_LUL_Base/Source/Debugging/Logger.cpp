#include "_LUL_Base.h"

// Logger ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void _LUL_::Logger::Start() noexcept
{
	if (!this->CreateTargetDir())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::CreateTargetDir() noexcept failed.");
	}
	if (!this->CreateOutTarget())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::CreateOutTarget() noexcept failed.");
	}
	if (!this->CleanOldData())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::CleanOldData() noexcept failed.");
	}
	if (!this->StartThread())
	{
		L_CONSOLE_OUT(L"LBOOL _LUL_::Logger::StartThread() noexcept failed.");
	}
}

// -----------------------------------------------------------------------------
void _LUL_::Logger::Log(Severity s, const LWCHAR* format, ...)
{
	namespace chrn = std::chrono;

#ifdef _DEBUG
	if (!m_bThreadStatus.load())
	{
		// Gate for a set amount of time, before repeating.
		static auto tpMs = chrn::time_point_cast<chrn::milliseconds>(chrn::system_clock::now());
		auto nowMs = chrn::time_point_cast<chrn::milliseconds>(chrn::system_clock::now());
		if ((nowMs.time_since_epoch() - tpMs.time_since_epoch()) >= chrn::milliseconds(1000))
		{
			L_CONSOLE_OUT(L"Logger thread isn't logging, but log was called.");

			// Update the gate
			tpMs = chrn::time_point_cast<chrn::milliseconds>(chrn::system_clock::now());
		}
	}
#endif // _DEBUG

	// Create the message.
	LWCHAR* formatBuff = new LWCHAR[LUL_MAX_LOG_SIZE]();

	va_list args;
	va_start(args, format);
	vswprintf_s(formatBuff, LUL_MAX_LOG_SIZE, format, args);
	va_end(args);

	m_MutexTLoop.lock();
	m_vMessages.Push(LOG_MESSAGE(s, formatBuff));
	m_MutexTLoop.unlock();
}

// -----------------------------------------------------------------------------
void _LUL_::Logger::Stop() noexcept
{
	if (!this->StopThread())
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
		std::this_thread::sleep_for(std::chrono::milliseconds(LogDelayMs));

		// try_lock let's us exit from the thread if the lock fails.
		if (!m_MutexTLoop.try_lock())
			continue;

		// Process the message

		m_MutexTLoop.unlock();
	}
}
