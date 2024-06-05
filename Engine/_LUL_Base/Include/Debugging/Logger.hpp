#ifndef LUL_LOGGER_H
#define LUL_LOGGER_H

#include <atomic>
#include <mutex>

namespace _LUL_
{
	enum LUL_API Severity
	{
		Info		= 0x00,
		Warning		= 0x02,
		Error		= 0x04,
		Exception	= 0x08
	};

	/**
	* Singleton that writes logs, 
	* creates and manages log files. */
	class LUL_API Logger
	{
		std::thread* m_tLogger = nullptr;

		Logger() noexcept = default;

	public:
		static Logger& Get()
		{
			static Logger instance;
			return instance;
		}
		
		~Logger() noexcept = default;

		Logger(Logger&) noexcept = default;
		Logger(Logger&&) = delete;

	public:
		void Start() noexcept;

		/**
		* Logs to the log file.
		* @param s - Severity of information
		* @param format - Log format
		* @param ... - Format arguments */
		void Log(Severity s, const LWCHAR* format, ...);

		void Stop() noexcept;

	private:
		LBOOL CreateTargetDir() noexcept;
		LBOOL CreateOutTarget() noexcept;
		LBOOL CleanOldData() noexcept;

		void Output();

		LBOOL StartThread() noexcept;
		LBOOL StopThread() noexcept;

		void SeparateThreadLoop();

	private:
		std::mutex m_MutexTLoop;

		std::atomic<LBOOL> m_bThreadStatus = false;

		_LUL_::LTuple<Severity, LWCHAR*, LCHAR*> m_

	};
}

#endif // !LUL_LOGGER_H
