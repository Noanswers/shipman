#include "stdafx.h"
#include "SystemClass.h"
#include "StartScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "Log.h"
#include "SkyObject.h"
#include "ResultObject.h"

bool CSystemClass::initialize()
{
	// Initialize the width and height of the screen to zero before sending the variables into the function.
	int screenWidth = 0;
	int screenHeight = 0;
	bool result;

	MyTime = new CMyTime();
	
	// Initialize the windows api.
	initializeWindows(screenWidth, screenHeight);

	/*GameManager = CGameManager::GetInstance();*/
	GameManager = new CGameManager();
	if (GameManager == nullptr)
		return false;

	SceneManager = CSceneManager::GetInstance();
	if (SceneManager == nullptr)
		return false;

	SceneManager->initialize();
	CMyScene* scene = SceneManager->getCurrentScene();
	scene->initialize();

	Input = CInputClass::GetInstance();
	Input->initialize();

	//Log initialize
	CLog* log = new CLog;
	log->initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	Graphics = new CGraphicsClass;
	if (!Graphics)
		return false;
		
	// Initialize the graphics object.
	result = Graphics->initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		log->SendErrorLogMessage("graphic initialize error!\n");
		return false;
	}
		
	return true;
}

void CSystemClass::shutdown()
{
	// Release the graphics object.
	if (Graphics)
	{
		CLog::GetInstance()->SendErrorLogMessage("shut down!\n");
		Graphics->shutdown();
		delete Graphics;
		Graphics = nullptr;
	}

	// Shutdown the window.
	shutdownWindows();

	return;
}

void CSystemClass::run()
{
	MSG msg;
	
	MyTime->Init();

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	bool done = false;
	while (!done)
	{
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
			bool result = frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

void CSystemClass::gameSceneInit()
{
	if (GameManager == nullptr)
		GameManager = new CGameManager();
	
	CGameScene* gameScene = new CGameScene();
	
	SceneManager->pushBack(gameScene);
	
	CMyScene* scene = SceneManager->getCurrentScene();
	
	//수정
	CStageObject* stageObject = new CStageObject();
	gameScene->pushBack(stageObject, 10);
	GameManager->setStage(stageObject);
	gameScene->initialize();

	initPlayerData(scene, 2);
	Graphics->gameScene = true;
}


bool CSystemClass::frame()
{
	bool result;
	CMyScene* currentScene = SceneManager->getCurrentScene();

	// 게임 종료 키입니다!
	if (Input->isKeyDown(VK_ESCAPE))
		return false;

	//Space 입력시 Scene변경 (임시)
	if (Input->isKeyDown(VK_SPACE))
	{
		if (dynamic_cast<CStartScene*>(currentScene))
		{
			Input->keyUp(VK_SPACE);
			CLog::GetInstance()->SendErrorLogMessage("Scene Change To GameScene!\n");
			gameSceneInit();
			Graphics->setCurrentInterval(0);
		}

		if (dynamic_cast<CResultScene*>(currentScene))
		{
			CLog::GetInstance()->SendErrorLogMessage("Scene Change To StartScene!\n");
			SceneManager->popBack();
			SceneManager->popBack();
			Input->keyUp(VK_SPACE);
			Graphics->setCameraStartScene();
			delete GameManager;
			GameManager = nullptr;
			for (auto& iter : PlayerDataVector)
			{
				delete std::get<CPlayerData*>(iter);
			}
			PlayerDataVector.clear();
			Graphics->gameScene = false;
		}
	}

	currentScene = SceneManager->getCurrentScene();

	// 플레이어의 입력을 받습니다
	getPlayerInput();
	
	// render 외에 scene 별로 정의된 연산을 수행합니다
	currentScene->doAction(0.1f);

	result = Graphics->frame(m_hwnd);
	if (!result)
		return false;

	if (dynamic_cast<CGameScene*>(currentScene))
	{
		result = GameManager->frame(PlayerVector);
		if (!result)
		{
			CLog::GetInstance()->SendErrorLogMessage("GameManager Frame Error!\n");
			return false;
		}

		if (GameManager->isGameEnd(PlayerVector))
		{
			CPlayerObject* player = GameManager->getWinnerPlayer(PlayerVector);
			endGameScene(player);
		}
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
		Input->keyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		Input->keyUp((unsigned int)wparam);
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

void CSystemClass::initPlayerData(CMyScene* scene, int playerNum)
{
	CLog::GetInstance()->SendErrorLogMessage("init player data\n");
	PlayerDataVector.clear();
	PlayerVector.clear();

	for (int i = 0; i < playerNum; ++i)
	{
		CPlayerObject* playerObj = new CPlayerObject();
		playerObj->setPlayerNumber(i+1);
		scene->pushBack(playerObj, 10);
	
		PlayerDataVector.push_back(std::make_tuple(new CPlayerData(), playerObj));
		PlayerVector.push_back(playerObj);
	}
	
	for (auto& iter : PlayerDataVector)
	{
		auto it = std::find(PlayerDataVector.begin(), PlayerDataVector.end(), iter);
		auto index = std::distance(PlayerDataVector.begin(), it);
		float theta = DirectX::XM_2PI*index / playerNum;

		std::get<CPlayerData*>(iter)->initialize();

		// OuterVector를 initialize합니다.
		std::get<CPlayerObject*>(iter)->setOuterVector(	-1.0f*cosf(theta), 0.0f, -1.0f*sinf(theta) );
		std::get<CPlayerObject*>(iter)->setOuterTheta(XMFLOAT3(0.0f, theta + XM_PI, 0.0f));

		// ForwardVector를 initialize합니다.
		std::get<CPlayerObject*>(iter)->setForwardVector( -1.0f*cosf(theta), 0.0f, -1.0f*sinf(theta) );		
		std::get<CPlayerObject*>(iter)->setForwardTheta( XMFLOAT3(0.0f, theta + XM_PI, 0.0f) );
		
		// 초기 위치와 방향을 initialize합니다.
		std::get<CPlayerObject*>(iter)->setTranslate(3.0f*cosf(theta), 0.0f, 3.0f*sinf(theta));
		std::get<CPlayerObject*>(iter)->setRotate(0.0f, 0.0f, 0.0f);
	}

	std::get<CPlayerData*>(PlayerDataVector[1])->setPlayerKeyUp(VK_W);
	std::get<CPlayerData*>(PlayerDataVector[1])->setPlayerKeyDown(VK_S);
	std::get<CPlayerData*>(PlayerDataVector[1])->setPlayerKeyLeft(VK_A);
	std::get<CPlayerData*>(PlayerDataVector[1])->setPlayerKeyRight(VK_D);
}

void CSystemClass::getPlayerInput()
{
	for (auto& iter : PlayerDataVector)
	{
		CPlayerData* player = std::get<CPlayerData*>(iter);
		KeySetting key = player->getPlayerKeySetting();
 		if (Input->isKeyDown(key.up))
 			std::get<CPlayerObject*>(iter)->accelerate();

		if (Input->isKeyDown(key.right))
			std::get<CPlayerObject*>(iter)->setRotate(0.0f, 0.7f, 0.0f);

		if (Input->isKeyDown(key.left))
			std::get<CPlayerObject*>(iter)->setRotate(0.0f, -0.7f, 0.0f);

		if (Input->isKeyDown(key.down))
			std::get<CPlayerObject*>(iter)->moveStop();
	}
}

void CSystemClass::endGameScene(CPlayerObject* winner)
{
	Graphics->setCameraStartScene();
	CResultScene* resultScene = new CResultScene();

	resultScene->initialize();
	CSceneManager::GetInstance()->pushBack(resultScene);

	CResultObject* resultObject = new CResultObject();
	resultObject->SetWinPlayerNum(winner->getPlayerNumber());
	resultScene->pushBack(resultObject, 10);
	resultObject->setTranslate(0.0f, 1.0f, 0.0f);
}