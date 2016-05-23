#include "stdafx.h"
#include <DDSTextureLoader.h>
#include "SkyObject.h"
#include "config.h"


bool CSkyObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (pTemp_Device != device)
	{
		pTemp_Device = device;
		IsInit = false;
	}

	if (textureFilename.empty() == true)
		textureFilename = textureSky;

	if (IsInit == true)
		return true;

	loadSkyTexture();
	createShader();
	createSphere(300.0f, 10, 10);
	createRenderState();
	createDepthStencil();

	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	return result;
}

void CSkyObject::shutdown()
{
	CMyObject::shutdown();
}

void CSkyObject::createShader()
{
	ID3DBlob* pErrorBlob = nullptr;
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(SkyFxFile.c_str(), 0, 0, "VS", "vs_5_0", 0,
		0, &pVSBlob, &pErrorBlob);

	hr = pTemp_Device->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		NULL, &SkyVertexShader);

	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);
	hr = pTemp_Device->CreateInputLayout(layout, numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&SkyVertexLayout);

	hr = D3DCompileFromFile(SkyFxFile.c_str(), 0,
		0, "PS", "ps_5_0", 0,
		0, &pPSBlob, &pErrorBlob);

	hr = pTemp_Device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		NULL,
		&SkyPixelShader);

	/*createSkyVertexShader(pVSBlob, pErrorBlob);
	createSkyVertexBufferLayout(pVSBlob);
	createSkyPixelShader(pPSBlob, pErrorBlob);*/

	pVSBlob->Release();
	pPSBlob->Release();
}

void CSkyObject::createSkyVertexShader(ID3DBlob* pVSBlob, ID3DBlob* pErrorBlob)
{
	HRESULT hr = D3DCompileFromFile(SkyFxFile.c_str(), 0, 0, "VS", "vs_5_0", 0,
		0, &pVSBlob, &pErrorBlob);

	hr = pTemp_Device->CreateVertexShader(
		pVSBlob->GetBufferPointer(), 
		pVSBlob->GetBufferSize(), 
		NULL, &SkyVertexShader);
}

void CSkyObject::createSkyPixelShader(ID3DBlob* pPSBlob, ID3DBlob* pErrorBlob)
{
	HRESULT hr = D3DCompileFromFile(SkyFxFile.c_str(), 0,
		0, "PS", "ps_5_0", 0,
		0, &pPSBlob, &pErrorBlob);

	hr = pTemp_Device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		NULL,
		&SkyPixelShader);
}

void CSkyObject::createSkyVertexBufferLayout(ID3DBlob* pVSBlob)
{
	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	UINT numElements = ARRAYSIZE(layout);
	HRESULT hr = pTemp_Device->CreateInputLayout(layout, numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&SkyVertexLayout);
}

// initializeBuffer function

bool CSkyObject::initializeBuffers(ID3D11Device* device)
{
	createSkyVertexBuffer();
	createSkyIndexBuffer();
	return true;
}

void CSkyObject::createSkyVertexBuffer()
{
	VertexType* vertices = &Verticies[0];

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VertexType) * Verticies.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;      			// vertex 정보가 담긴 vector
	pTemp_Device->CreateBuffer(&bd, &InitData, &SkyVertBuffer);
}

void CSkyObject::createSkyIndexBuffer()
{
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.ByteWidth = sizeof(UINT) * Indices.size();
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA iinitData;
	ZeroMemory(&iinitData, sizeof(iinitData));
	iinitData.pSysMem = &Indices[0];
	pTemp_Device->CreateBuffer(&ibd, &iinitData, &SkyIndexBuffer);
}

void CSkyObject::loadSkyTexture()
{
	CreateDDSTextureFromFile(pTemp_Device, textureFilename.c_str(), &Resource, &pTextureRV, NULL);
}

void CSkyObject::createRenderState()
{
	D3D11_RASTERIZER_DESC      rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;             // Sky Dome 안쪽이 보여야 한다.
	rasterizerDesc.FrontCounterClockwise = false;
	
	pTemp_Device->CreateRasterizerState(&rasterizerDesc,  &SolidRS);
}

void CSkyObject::createDepthStencil()
{
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
	pTemp_Device->CreateDepthStencilState(&dssDesc, &DepthStencilLessEqual);
}

void CSkyObject::createSphere(float radius, int sliceCount, int stackCount)
{
	Verticies.clear();
	Indices.clear();

	Verticies.push_back({ 
		XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT2(0.0f, 0.0f) 
	});

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f*DirectX::XM_PI / sliceCount;

	for (int i = 0; i < stackCount; ++i) 
	{
		float phi = (i+1)*phiStep;
		for (int j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;
			Verticies.push_back({
				XMFLOAT3(radius* sinf(phi)*cosf(theta), radius* cosf(phi), radius* sinf(phi)*sinf(theta)),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				XMFLOAT3(radius* sinf(phi)*cosf(theta), radius* cosf(phi), radius* sinf(phi)*sinf(theta)),
				XMFLOAT2(0.0f, 0.0f)
			});

		//http://richardssoftware.net/Home/Post/7
		}
	}
	Verticies.push_back({
		XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT2(0.0f, 0.0f) 
	});

	for (int i = 1; i <= sliceCount; i++) {
		Indices.push_back(0);
		Indices.push_back(i);
		Indices.push_back(i + 1);
		
	}
	int baseIndex = 1;
	int ringVertexCount = sliceCount + 1;
	for (int i = 0; i < stackCount - 2; i++) {
		for (int j = 0; j < sliceCount; j++) {
			Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			Indices.push_back(baseIndex + i*ringVertexCount + j);
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
					
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
			Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
		}
	}
	int southPoleIndex = Verticies.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount;
	for (int i = 0; i < sliceCount; i++) {
		Indices.push_back(southPoleIndex);
		Indices.push_back(baseIndex + i + 1);
		Indices.push_back(baseIndex + i);
	}
}

void CSkyObject::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType);;
	unsigned int offset = 0;

	deviceContext->OMSetDepthStencilState(DepthStencilLessEqual, 0);

	deviceContext->IASetInputLayout(SkyVertexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->RSSetState(SolidRS);
	
	deviceContext->VSSetShader(SkyVertexShader, NULL, 0);
	deviceContext->PSSetShader(SkyPixelShader, NULL, 0);
	//텍스쳐!
	deviceContext->PSSetShaderResources(0, 1, &pTextureRV);
	deviceContext->PSSetSamplers(0, 1, &pSamplerLinear);

	// 생성된 버퍼의 정점들을 실제로 파이프라인으로 공급하려면 버퍼를 장치의 한 입력 슬롯에 묶어야 함
	// 1번째 인자 : 
	deviceContext->IASetVertexBuffers(0, 1, &SkyVertBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(SkyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.

	deviceContext->DrawIndexed(Indices.size(), 0, 0);

	return;
}