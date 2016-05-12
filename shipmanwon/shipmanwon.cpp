// shipmanwon.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "shipmanwon.h"
#include "SystemClass.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	CSystemClass* System;
	bool result;

	// Create the system object.
	System = new CSystemClass;
	if (!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->initialize();
	if (result)
	{
		System->run();
	}

	// Shutdown and release the system object.
	System->shutdown();
	delete System;
	System = nullptr;

	return 0;
}