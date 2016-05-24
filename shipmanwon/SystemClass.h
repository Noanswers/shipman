#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <vector>
#include <tuple>
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SceneManager.h"
#include "MyScene.h"
#include "MyLayer.h"
#include "MyObject.h"
#include "GameManager.h"
#include "MyTime.h"
#include "PlayerData.h"
#include "StartObject.h"
#include "StageObject.h"
#include "SoundManager.h"

class CSystemClass
{
public:
	CSystemClass() = default;
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
	void initPlayerData(CMyScene * scene, int playerNum);
	void getPlayerInput();
	void endGameScene(CPlayerObject * winner);
	void gameSceneInit();

	//	variables
	std::vector<std::tuple<CPlayerData*, CPlayerObject*>> PlayerDataVector;
	std::vector<CPlayerObject*> PlayerVector;

	CGraphicsClass* Graphics = nullptr;
	CInputClass*	Input = nullptr;
	CMyTime*		MyTime = nullptr;

	CGameManager*	GameManager = nullptr;
	CSceneManager*	SceneManager = nullptr;

	LPCWSTR			m_applicationName;
	HINSTANCE		m_hinstance;
	HWND			m_hwnd;
	CSoundManager*	SoundManager;
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