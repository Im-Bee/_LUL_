#pragma once

#include "../../../Engine/_LUL_Base/Include/_LUL_Base.h"
#include "../../../Engine/_LUL_Base/Include/EntryPoint/Win32/Entry.hpp"

#include <chrono>

class Application
	: public _LUL_::IApplication
{
public:
	void Initialize() override 
	{

	}
	void Update() override {}
	void Destroy() override {}
};

LUL_DECLARE_APPLICATION(Application );