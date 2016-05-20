#include "stdafx.h"
#include "GraphicsClass.h"
#include "SkyObject.h"

bool CGraphicsClass::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Create the Direct3D object.
	m_Direct3D = new CD3DClass;
	if (!m_Direct3D)
		return false;

	// Initialize the Direct3D object.
	bool result = m_Direct3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CCameraClass();
	if (!m_Camera)
		return false;

	setCameraStartScene();
	createConstantBuffer();
	createSkyConstantBuffer();

	return true;
}

void CGraphicsClass::shutdown()
{
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
	if (m_pConstantBuffer) m_pConstantBuffer->Release();

	return;
}

bool CGraphicsClass::frame(HWND hWnd)
{
	if(gameScene == true && (CurrentInterval < CameraMoveInterval))
		setCameraGameScene();

	bool result = render(hWnd);
	
	return result;
}

bool CGraphicsClass::render(HWND hWnd)
{
	/*XMMATRIX worldMatrix, viewMatrix, projectionMatrix;*/
	bool result;

	// 랜더 타겟 뷰와 뎁스 스탠실 뷰를 클리어 합니다.
	float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	CSceneManager* sm = CSceneManager::GetInstance();

	CMyScene* scene = CSceneManager::GetInstance()->getCurrentScene();
	scene->getSceneColor(color);

	//m_Direct3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_Direct3D->beginScene(color[0], color[1], color[2], color[3]);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	calculateMatrixForCB();

	result = scene->initScene(m_Direct3D->getDevice(), hWnd);
	if (!result)
	{
		return false;
	}

	result = renderCurrentScene();
	if (!result)
	{
		return false;
	}

	// test
	/*CMyObject* bg = new CSkyObject();
	bg->initialize(m_Direct3D->getDevice(), hWnd);
	std::function<bool(ID3D11DeviceContext*, CMyObject*)> setSkyShader(std::bind(&CGraphicsClass::setShaderParameters, this, std::placeholders::_1, std::placeholders::_2));
	bg->renderObject(m_Direct3D->getDeviceContext(), setSkyShader);*/


	// 백 버퍼와 프론트 버퍼를 교체해줍니다.
	m_Direct3D->endScene();

	return true;
}

void CGraphicsClass::createConstantBuffer()
{
	D3D11_BUFFER_DESC 	cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	m_Direct3D->getDevice()->CreateBuffer(&cbd, NULL, &m_pConstantBuffer);
}

void CGraphicsClass::createSkyConstantBuffer()
{
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd,   sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(skyConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	m_Direct3D->getDevice()->CreateBuffer(&cbd, NULL, &SkyConstantBuffer);
}


void CGraphicsClass::calculateMatrixForCB()
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	m_Direct3D->getWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->getProjectionMatrix(projectionMatrix);

	ID3D11DeviceContext* deviceContext = m_Direct3D->getDeviceContext();
	// 박스를 회전시키기 위한 연산.    위치, 크기를 변경하고자 한다면 SRT를 기억할 것.      

	DirectX::XMMATRIX wvp = worldMatrix * viewMatrix * projectionMatrix;
	ConstantBuffer       cb;

	cb.wvp = XMMatrixTranspose(wvp);
	cb.world = XMMatrixTranspose(worldMatrix);
	cb.lightDir = lightDirection;
	cb.lightColor = lightColor;

	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &cb, 0, 0); // update data
	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);// set constant buffer.
	deviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);// set constant buffer to PS.
}

bool CGraphicsClass::renderCurrentScene()
{
	CMyScene* currentScene = CSceneManager::GetInstance()->getCurrentScene();
	
	std::function<bool(ID3D11DeviceContext*, CMyObject*)> testFunc(std::bind(&CGraphicsClass::setShaderParameters, this, std::placeholders::_1, std::placeholders::_2));

	bool result = currentScene->renderScene(m_Direct3D->getDeviceContext(), testFunc);
	return result;
}

bool CGraphicsClass::setShaderParameters(ID3D11DeviceContext* deviceContext, CMyObject* object)
{
	if (m_pConstantBuffer == nullptr)
		return false;

	// Transpose the matrices to prepare them for the shader.
	DirectX::XMMATRIX worldMatrix = object->getWorldMatrix();
	
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();
	m_Camera->GetViewMatrix(viewMatrix);
	
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixIdentity();
	m_Direct3D->getProjectionMatrix(projectionMatrix);
	
	DirectX::XMMATRIX orthogonalMatrix = DirectX::XMMatrixIdentity();
	m_Direct3D->getOrthoMatrix(orthogonalMatrix);
	
	// Finanly set the constant buffer in the vertex shader with the updated values.
	DirectX::XMMATRIX wvp = worldMatrix * viewMatrix * projectionMatrix;
	ConstantBuffer       cb;

	cb.wvp = DirectX::XMMatrixTranspose(wvp);
	cb.world = DirectX::XMMatrixTranspose(worldMatrix);
	cb.lightDir = lightDirection;
	cb.lightColor = lightColor;

	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &cb, 0, 0); // update data
	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);// set constant buffer to PS.

	return true;
}

bool CGraphicsClass::setSkyShaderParameters(ID3D11DeviceContext* deviceContext, CMyObject* object)
{
	if (m_pConstantBuffer == nullptr)
		return false;

	// Transpose the matrices to prepare them for the shader.
	DirectX::XMMATRIX worldMatrix = object->getWorldMatrix();

	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();
	m_Camera->GetViewMatrix(viewMatrix);

	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixIdentity();
	m_Direct3D->getProjectionMatrix(projectionMatrix);

	DirectX::XMMATRIX orthogonalMatrix = DirectX::XMMatrixIdentity();
	m_Direct3D->getOrthoMatrix(orthogonalMatrix);

	// Finanly set the constant buffer in the vertex shader with the updated values.
	DirectX::XMMATRIX wvp = worldMatrix * viewMatrix * projectionMatrix;
	ConstantBuffer       cb;

	cb.wvp = DirectX::XMMatrixTranspose(wvp);

	deviceContext->UpdateSubresource(SkyConstantBuffer, 0, 0, &cb, 0, 0); // update data
	deviceContext->VSSetConstantBuffers(0, 1, &SkyConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &SkyConstantBuffer);// set constant buffer to PS.

	return true;
}

void CGraphicsClass::setCameraStartScene()
{
	m_Camera->SetPosition(0.0f, 0.0f, -12.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
}

void CGraphicsClass::setCameraGameScene()
{
	DirectX::XMFLOAT3 destPos = { 0.0f, 11.0f, -6.0f };
	DirectX::XMFLOAT3 destRotate = { 60.0f, 0.0f, 0.0f };

	DirectX::XMFLOAT3 pos = m_Camera->GetPosition();
	DirectX::XMFLOAT3 rotate = m_Camera->GetRotation();

	m_Camera->SetPosition(
		pos.x + (destPos.x - pos.x)*CurrentInterval / CameraMoveInterval,
		pos.y + (destPos.y - pos.y)*CurrentInterval / CameraMoveInterval,
		pos.z + (destPos.z - pos.z)*CurrentInterval / CameraMoveInterval
	);
	m_Camera->SetRotation(
		rotate.x + (destRotate.x - rotate.x)*CurrentInterval / CameraMoveInterval,
		rotate.y + (destRotate.y - rotate.y)*CurrentInterval / CameraMoveInterval,
		rotate.z + (destRotate.z - rotate.z)*CurrentInterval / CameraMoveInterval
	);
	++CurrentInterval;
	/*m_Camera->SetPosition(0.0f, 9.0f, -6.0f);
	m_Camera->SetRotation(60.0f, 0.0f, 0.0f);*/
};
	