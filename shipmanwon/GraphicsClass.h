#pragma once
#include <windows.h>

// GLOBALS
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class CGraphicsClass
{
public:
	CGraphicsClass() = default;
	~CGraphicsClass() = default;
	
	CGraphicsClass(const CGraphicsClass&) = delete;

	bool initialize(int, int, HWND);
	void shutdown();
	bool frame();

private:
	bool render();

};

