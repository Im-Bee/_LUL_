#ifndef LUL_ENTRY_H
#define LUL_ENTRY_H

#if _WIN32 || _WIN64

INT CALLBACK wWinMain(
	_In_	 HINSTANCE hInst, 
	_In_opt_ HINSTANCE hInstPrev,
	_In_	 LPWSTR cmdline, 
	_In_	 INT cmdshow)
{
	auto& appMan = _LUL_::AppManager::Get();
	MSG msg;
	
	try 
	{
		appMan.StartApplication(_LUL_::EntryApplication());
	}
	catch (_LUL_::LException e)
	{
		appMan.ExitWithErrorMsg(e.What());
	}
	catch (std::exception e)
	{
		appMan.ExitWithErrorMsg(e.what());
	}
	catch (...)
	{
		appMan.ExitWithErrorMsg("Unhandled exception.");
	}

	// Main application loop
	// msg = { 0 };
	// while (msg.message != WM_QUIT)
	// {
	// 	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	// 	{
	// 		TranslateMessage(&msg);
	// 		DispatchMessage(&msg);
	// 	}
	// 
	// 	appMan.UpdateApplication();
	// }

	_LUL_::AppManager::Get().CloseApplication(false);
}

#else
#	error "Included application entry point isn't available on this system."
#endif
#endif // !LUL_ENTRY_H
