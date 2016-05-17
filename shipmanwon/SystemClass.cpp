#include "stdafx.h"
#include "SystemClass.h"
#include "StartScene.h"
#include "GameScene.h"
#include "Log.h"

CSystemClass::CSystemClass()
{
	Graphics = nullptr;
}

bool CSystemClass::initialize()
{
	// Initialize the width and height of the screen to zero before sending the variables into the function.
	int screenWidth = 0;
	int screenHeight = 0;
	bool result;

	MyTime = new CMyTime();
	
	// Initialize the windows api.
	initializeWindows(screenWidth, screenHeight);

	// GameManager를 생성합니다.
	GameManager = CGameManager::GetInstance();
	if (GameManager == nullptr)
	{
		return false;
	}

	SceneManager = CSceneManager::GetInstance();
	if (SceneManager == nullptr)
	{
		return false;
	}

	SceneManager->initialize();
	CMyScene* scene = SceneManager->getCurrentScene();
	
	CPlayerObject* pObj1 = new CPlayerObject();
	CPlayerObject* pObj2 = new CPlayerObject();

	scene->pushBack(pObj1, 10);
	scene->pushBack(pObj2, 10);
	
	//에러의 원흉
	PlayerDataVector.push_back(std::make_tuple(new CPlayerData(), pObj1));
	PlayerDataVector.push_back(std::make_tuple(new CPlayerData(), pObj2));

	int i = 1;
	for (auto& iter : PlayerDataVector)
	{
		std::get<CPlayerData*>(iter)->initialize();
		std::get<CPlayerObject*>(iter)->setScale(0.5f, 0.2f, 0.5f);
		std::get<CPlayerObject*>(iter)->setTranslate(3.0f*i, 0.0f, 0.0f);
		i = -i;
	}
	std::get<CPlayerData*>(PlayerDataVector[1])->setPlayerKeyUp(VK_W);
	std::get<CPlayerData*>(PlayerDataVector[1])->setPlayerKeyLeft(VK_A);
	std::get<CPlayerData*>(PlayerDataVector[1])->setPlayerKeyRight(VK_D);

	scene->initialize();
	
	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	Input = CInputClass::GetInstance();

	// Initialize the input object.
	CInputClass::GetInstance()->initialize();

	//Log initialize

	CLog* log = new CLog;
	log->initialize();
	log->SendErrorLogMessage("test\n");
	log->SendErrorLogMessage("test2\n");

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	Graphics = new CGraphicsClass;
	if (!Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = Graphics->initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void CSystemClass::shutdown()
{
	// Release the graphics object.
	if (Graphics)
	{
		Graphics->shutdown();
		delete Graphics;
		Graphics = nullptr;
	}

	// Release the input object.
	/*if (Input)
	{
		Input->DestorySingleton();
	}

	/*if (GameManager)
	{
		GameManager->DestorySingleton();
	}*/

	// Shutdown the window.
	shutdownWindows();

	return;
}

void CSystemClass::run()
{
	MSG msg;
	bool done, result;
	
	MyTime->Init();

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		MyTime->ProcessTime();
		float delta = MyTime->GetElapsedTime();
		static float deltaTime = 0;
		deltaTime += delta;

		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing. ( Input + Graphic + logic )
			result = frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}

bool CSystemClass::frame()
{
	// Check if the user pressed escape and wants to exit the application.
	if (CInputClass::GetInstance()->isKeyDown(VK_ESCAPE))
	{
		return false;
	}

	for (auto& iter : PlayerDataVector)
	{
		CPlayerData* player = std::get<CPlayerData*>(iter);
		KeySetting key = player->getPlayerKeySetting();

		//여기서 플레이어마다 지정된 키가 눌렸을 때 특정 동작을 할당
		if (Input->isKeyDown(key.up))
			std::get<CPlayerObject*>(iter)->moveForward();

		if (Input->isKeyDown(key.right))
			std::get<CPlayerObject*>(iter)->setRotate(0.0f, 1.0f, 0.0f);

		if (Input->isKeyDown(key.left))
			std::get<CPlayerObject*>(iter)->setRotate(0.0f, -1.0f, 0.0f);
	}
	
	bool result = GameManager->frame();
	if (!result)
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = Graphics->frame(m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK CSystemClass::messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		CInputClass::GetInstance()->keyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		CInputClass::GetInstance()->keyUp((unsigned int)wparam);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void CSystemClass::initializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.  
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void CSystemClass::shutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}