#include "stdafx.h"
#include "GraphicsClass.h"

bool CGraphicsClass::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_Direct3D = new CD3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CCameraClass();
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	/*m_Camera->SetPosition(10.0f, -10.0f, -10.0f);
	m_Camera->SetRotation(-45.0f, -45.0f, 45.0f);*/
	
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);

	// Create the color shader object.
	m_ColorShader = new CColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_Direct3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void CGraphicsClass::shutdown()
{
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

bool CGraphicsClass::frame()
{
	bool result;

	// Render the graphics scene.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool CGraphicsClass::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// 랜더 타겟 뷰와 뎁스 스탠실 뷰를 클리어 합니다.
	m_Direct3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->getWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->getProjectionMatrix(projectionMatrix);

	/*
		매번 랜더링하기 전에 init이 된 object가 없는지 확인하고, 
		initialize가 되지 않은 경우 초기화 진행

		그 후, 랜더링 작업
	*/

	CMyScene* scene = CSceneManager::GetInstance()->getCurrentScene();
	result = scene->initScene(m_Direct3D->getDevice());
	if (!result)
	{
		return false;
	}

	CMyScene* currentScene = CSceneManager::GetInstance()->getCurrentScene();
	currentScene->renderScene(m_Direct3D->getDeviceContext());

	// Render the model using the color shader.

	result = m_ColorShader->Render(m_Direct3D->getDeviceContext(), 3, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// 백 버퍼와 프론트 버퍼를 교체해줍니다.
	m_Direct3D->endScene();

	return true;
}