#pragma once
#include <windows.h>
#include "d3dclass.h"
#include "SceneManager.h"
#include "cameraclass.h"

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
	bool frame(HWND hWnd);
	bool setShaderParameters(ID3D11DeviceContext* deviceContext, CMyObject* object);
	bool setSkyShaderParameters(ID3D11DeviceContext * deviceContext, CMyObject * object);
	
//	임시 카메라 세팅
	void setCameraStartScene();
	//void setCameraGameScene();
	void setCameraGameScene();

	bool gameScene = false;
private:
	bool render(HWND hWnd);
	bool renderCurrentScene();

	void createConstantBuffer();
	void calculateMatrixForCB();


	int CurrentInterval = 0;
	float CameraMoveInterval = 500;


	CSceneManager* SceneManager = nullptr;
	CD3DClass* m_Direct3D = nullptr;
	CCameraClass* m_Camera = nullptr;

	ID3D11Buffer* m_pConstantBuffer;

	XMFLOAT4 lightDirection =
	{
		XMFLOAT4(1.0f, -0.5f, 1.0f, 1.0f)
	};

	XMFLOAT4 lightColor =
	{
		XMFLOAT4(0.8f, 0.8f, 1.0f, 1.0f)
	};

	struct ConstantBuffer
	{
		XMMATRIX	wvp;
		XMMATRIX	world;

		XMFLOAT4	lightDir;
		XMFLOAT4	lightColor;
	};

	ID3D11Buffer * SkyConstantBuffer;
	void createSkyConstantBuffer();

	struct    skyConstantBuffer
	{
		XMMATRIX wvp;
	};
};


