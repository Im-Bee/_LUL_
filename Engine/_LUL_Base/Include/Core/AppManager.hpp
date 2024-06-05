#ifndef LUL_APP_MANAGER_H
#define LUL_APP_MANAGER_H

#include "Core/IApplication.hpp"

namespace _LUL_
{
	/**
	* Singleton that manages the app life time.
	* Manages app configuartion, basic app resources
	* and config associated files. */
	class LUL_API AppManager
	{
		IApplication* m_pAppInstance = nullptr;
		LBOOL m_bAppInitialized = false;

		AppManager() noexcept = default;

	public:
		static AppManager& Get()
		{
			static AppManager instance;
			return instance;
		}

		~AppManager() noexcept = default;

		AppManager(AppManager&) noexcept = default;
		AppManager(AppManager&&) = delete;

	public:
		/**
		* Create application instance etc. */
		void StartApplication(IApplication* pEntryapp);

		void UpdateApplication();

		/**
		* Destroy application instance. 
		* @param exit - If set to true then exits the program completly. */
		void CloseApplication(LBOOL bExit = true);
		
		/**
		* Exit application completly 
		* and display an error message box with a message. */
		static void ExitWithErrorMsg(LCHAR const* const sMsg) noexcept;

	public:
		/**
		* Create a file. */
		LBOOL Create(LWCHAR* sPath);

		/**
		* Create a file in known path.
		* @param sSubPath - Sub path in relate to known path.
		* @param kp - Known path from KnownPaths enum. 
		* @param (Optional) sCombinedPath - Combined path of sub path and known path. */
		LBOOL CreateInKnownPath(
			LWCHAR* sSubPath, 
			const KnownPaths& kp,
			LWCHAR* sCombinedPath);

		static void StopLUL();
	};
}

#endif // !LUL_APP_MANAGER_H
