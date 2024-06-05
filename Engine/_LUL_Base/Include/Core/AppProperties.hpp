#ifndef LUL_APP_PROPS_H
#define LUL_APP_PROPS_H

#include "AppManager.hpp"

namespace _LUL_
{
	/**
	* Singleton that stores buffers 
	* to corresponding properites of application.
	* Icons, app specific strings, boot time etc. */
	class LUL_API AppProps
	{
		friend AppManager;

		LTIME m_BootTime = { 0 };
		const LWCHAR* m_KnownPaths[LUL_AMOUNT_OF_KNOWN_PATHS] = { 0 };

		AppProps() noexcept = default;

	public:
		static AppProps& Get()
		{
			static AppProps instance;
			return instance;
		}

		~AppProps() noexcept = default;

		AppProps(AppProps&) noexcept = default;
		AppProps(AppProps&&) = delete;

	public:
		const LTIME& GetBootTime() { return m_BootTime; };

		LWCHAR const* const GetKnownPath(const KnownPaths& kp);

	private:
		LBOOL UpdateBootTime();

		void UpdateKnownPaths();

	};
}

#endif // !LUL_APP_PROPS_H
