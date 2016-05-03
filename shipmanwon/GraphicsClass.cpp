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

	return true;
}

void CGraphicsClass::shutdown()
{
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

//	여기서 Scene 작업을 하면 됩니다.
//	Scene Stack으로부터 Scene을 받아와서 그리도록 합니다.
bool CGraphicsClass::render()
{
	// Clear the buffers to begin the scene.
	m_Direct3D->beginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Present the rendered scene to the screen.
	m_Direct3D->endScene();

	return true;
}