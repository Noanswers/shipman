#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SceneManager.h"
#include "MyScene.h"
#include "MyLayer.h"
#include "MyObject.h"
#include "GameManager.h"
#include "MyTime.h"


class CSystemClass
{
public:
	CSystemClass();
	CSystemClass(const CSystemClass&) = delete;
	~CSystemClass() = default;

	bool initialize();
	void shutdown();
	void run();

	LRESULT CALLBACK messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
//	function
	bool frame();
	void initializeWindows(int& screenWidth, int& screenHeight);
	void shutdownWindows();

//	variables
	CGraphicsClass* Graphics;
	CInputClass*	Input;
	CMyTime*		MyTime;

	CGameManager*	GameManager;
	CSceneManager*	SceneManager;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
};

static CSystemClass* ApplicationHandle = nullptr;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->messageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}