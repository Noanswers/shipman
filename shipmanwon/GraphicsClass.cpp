#include "stdafx.h"
#include "GraphicsClass.h"

bool CGraphicsClass::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Create the Direct3D object.
	m_Direct3D = new CD3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

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
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, -9.0f, -6.0f);
	m_Camera->SetRotation(-60.0f, 0.0f, 0.0f);

	createConstantBuffer();

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
	bool result = render(hWnd);
	
	return result;
}

bool CGraphicsClass::render(HWND hWnd)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
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

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->getWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->getProjectionMatrix(projectionMatrix);

	calculateMatrixForCB();

	result = scene->initScene(m_Direct3D->getDevice(), hWnd);
	if (!result)
	{
		return false;
	}

	CMyScene* currentScene = CSceneManager::GetInstance()->getCurrentScene();
	result = currentScene->renderScene(m_Direct3D->getDeviceContext());
	if (!result)
	{
		return false;
	}

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

void CGraphicsClass::calculateMatrixForCB()
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	m_Direct3D->getWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->getProjectionMatrix(projectionMatrix);

	ID3D11DeviceContext* deviceContext = m_Direct3D->getDeviceContext();
	// 박스를 회전시키기 위한 연산.    위치, 크기를 변경하고자 한다면 SRT를 기억할 것.      

	XMMATRIX wvp = worldMatrix * viewMatrix * projectionMatrix;
	ConstantBuffer       cb;

	cb.wvp = XMMatrixTranspose(wvp);
	cb.world = XMMatrixTranspose(worldMatrix);
	cb.lightDir = lightDirection;
	cb.lightColor = lightColor;

	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &cb, 0, 0); // update data
	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);// set constant buffer.
	deviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);// set constant buffer to PS.
}

bool CGraphicsClass::setShaderParameters(ID3D11DeviceContext* deviceContext, CMyObject* object)
{
	if (m_pConstantBuffer == nullptr)
		return false;

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstantBuffer* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	DirectX::XMMATRIX worldMatrix = XMMatrixTranspose(object->getWorldMatrix());
	
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();
	m_Camera->GetViewMatrix(viewMatrix);
	XMMatrixTranspose(viewMatrix);

	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixIdentity();
	m_Direct3D->getProjectionMatrix(projectionMatrix);
	XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (ConstantBuffer*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->wvp = worldMatrix * viewMatrix * projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_pConstantBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pConstantBuffer);

	return true;
}