#include "stdafx.h"
#include "PlayerObject.h"
#include "config.h"
#include "InputClass.h"
#include <directxmath.h>

bool CPlayerObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (temp_device != device)
	{
		temp_device = device;
		IsInit = false;
	}

	if (textureFilename.empty() == true)
		textureFilename = textureDefault;

	if (IsInit == true)
		return true;

	loadTexture();
	createShader();
	createCylinder();

	bool result = initializeBuffers(device, vertices, indices, m_vertexCount, m_indexCount);
	if (result == true)
		IsInit = true;

	return result;
}

void CPlayerObject::shutdown()
{
	//CMyObject::shutdown();
	shutdownBuffers();

	return;
}

bool CPlayerObject::renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc)
{
	bool result = true;
	
	setShaderfunc(deviceContext, this);
	renderBuffers(deviceContext);

	return result;
}

HRESULT CPlayerObject::loadTexture()
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(temp_device, textureFilename.c_str(), &Resource, &m_pTextureRV, NULL);

	if (FAILED(hr))
		return hr;

	D3D11_SAMPLER_DESC	sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = temp_device->CreateSamplerState(&sampDesc, &m_pSamplerLinear);	// SamplerState 생성
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void CPlayerObject::createShader()
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pVSBlob = NULL;
	HRESULT hr = D3DCompileFromFile(
		FxFile.c_str(), 0, 0,
		"VS", "vs_5_0",
		0, 0,
		&pVSBlob, &pErrorBlob);

	//버텍스 쉐이더 생성
	hr = temp_device->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		0, &m_pVertexShader);

	//	해당 D3D_INPUT_ELEMENT_DESC 를 vertex의 struct에 맞춰서 수정해야 함

	D3D11_INPUT_ELEMENT_DESC	layout[] =
	{
		//4번째 인자는 시작 index
		//D3D11_APPEND_ALIGHNED_ELEMENT의 경우 시작 offset 자동 계산

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }

		//	D3D11_APPEND_ALIGNED_ELEMENT 사용시 offset 자동 계산이 됩니다.
	};

	UINT numElements = ARRAYSIZE(layout);

	hr = temp_device->CreateInputLayout(layout, numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&m_pVertexLayout);

	pVSBlob->Release();

	ID3DBlob* pPSBlob = NULL;

	hr = D3DCompileFromFile(FxFile.c_str(), 0, 0,			// Shader 파일 설정
		"PS", "ps_5_0",											// 컴파일 설정
		0, 0,												// 쉐이더 옵션
		&pPSBlob, &pErrorBlob);								// 리턴

	hr = temp_device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		0, &m_pPixelShader);

	pPSBlob->Release();
}

void CPlayerObject::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType);;
	unsigned int offset = 0;

	deviceContext->IASetInputLayout(m_pVertexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//텍스쳐!
	deviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	deviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

	// 생성된 버퍼의 정점들을 실제로 파이프라인으로 공급하려면 버퍼를 장치의 한 입력 슬롯에 묶어야 함
	// 1번째 인자 : 
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->DrawIndexed(cyIndices.size(), 0, 0);

	return;
}

bool CPlayerObject::initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices, int m_vertexCount, int m_indexCount)
{
	m_vertexCount = cyVerticies.size();
	m_indexCount = cyIndices.size();

	vertices = &cyVerticies[0];
	indices = &cyIndices[0];

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;

	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CPlayerObject::createCylinder()
{
	cyVerticies.clear();
	cyIndices.clear();

	float stackHeight = height / stackCount;

	float radiusStep = (topRadius - bottomRadius) / stackCount;

	UINT ringCount = stackCount + 1;

	for (UINT i = 0; i < ringCount; ++i)
	{
		float y = i*stackHeight;
		//float y = -0.5f*height + i*stackHeight;
		float r = bottomRadius + i*radiusStep;

		float dTheta = 2.0f * DirectX::XM_PI / sliceCount;

		for (UINT j = 0; j <= sliceCount; ++j)
		{
			VertexType vertex;

			float myCos = cosf(j*dTheta);
			float mySin = sinf(j*dTheta);

			vertex.position = DirectX::XMFLOAT3(r*myCos, y, r*mySin);

			vertex.tex.x = (float)j / sliceCount;
			vertex.tex.y = 1.0f - (float)i / stackCount;

			vertex.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex.normal = vertex.position;

			//verteex.TangentU 필요한지 확인할것
			cyVerticies.push_back(vertex);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//make indicies
	UINT ringVertexCount = sliceCount + 1;

	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			cyIndices.push_back(i*ringVertexCount + j);
			cyIndices.push_back((i+1)*ringVertexCount + j);
			cyIndices.push_back((i+1)*ringVertexCount + j+1);

			cyIndices.push_back(i*ringVertexCount + j);
			cyIndices.push_back((i+1)*ringVertexCount + j+1);
			cyIndices.push_back(i*ringVertexCount + j+1);
		}
	}

	createTopCap();
	createBottomCap();
}

void CPlayerObject::createTopCap()
{
	UINT baseIndex = (UINT)cyVerticies.size();

	float y = height;
	//float y = 0.5f*height;
	float dTheta = 2.0f*DirectX::XM_PI / sliceCount;

	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius*cosf(i*dTheta);
		float z = topRadius*sinf(i*dTheta);

		//texture coordinate
		float u = x / height;
		float v = z / height;

		VertexType vertex;
		vertex.position = DirectX::XMFLOAT3(x, y, z);
		vertex.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex.normal = DirectX::XMFLOAT3(0.0f, y, 0.0f);
		vertex.tex = DirectX::XMFLOAT2(u, v);

		cyVerticies.push_back(vertex);

		/*cyVerticies.push_back( {DirectX::XMFLOAT3(x, y, z), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(u, v) });*/
	}

	//center of cap vertex
	cyVerticies.push_back({ DirectX::XMFLOAT3(0.0f, y, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });

	//center of cap index
	UINT centerIndex = (UINT)cyVerticies.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		cyIndices.push_back(centerIndex);
		cyIndices.push_back(baseIndex + i + 1);
		cyIndices.push_back(baseIndex + i);
	}
}

void CPlayerObject::createBottomCap()
{
	UINT baseIndex = (UINT)cyVerticies.size();

	float y = 0.0f;
	//float y = -0.5f*height;
	float dTheta = 2.0f*DirectX::XM_PI / sliceCount;

	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius*cosf(i*dTheta);
		float z = topRadius*sinf(i*dTheta);

		//texture coordinate
		float u = x / height;
		float v = z / height;

		VertexType vertex;
		vertex.position = DirectX::XMFLOAT3(x, y, z);
		vertex.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex.normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertex.tex = DirectX::XMFLOAT2(u, v);

		cyVerticies.push_back(vertex);

		/*cyVerticies.push_back({ DirectX::XMFLOAT3(x, y, z), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(u, v) });*/
	}

	//center of cap vertex
	cyVerticies.push_back({ DirectX::XMFLOAT3(0.0f, y, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });

	//center of cap index
	UINT centerIndex = (UINT)cyVerticies.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		cyIndices.push_back(centerIndex);
		cyIndices.push_back(baseIndex + i);
		cyIndices.push_back(baseIndex + i + 1);
	}
}

void CPlayerObject::update()
{
	//if (CInputClass::GetInstance()->isKeyDown(VK_LEFT))
	//	setTranslate(vertices->position.x -= speed, vertices->position.y, vertices->position.z);
	//
	//if (CInputClass::GetInstance()->isKeyDown(VK_RIGHT))
	//	setTranslate(vertices->position.x += speed, vertices->position.y, vertices->position.z);
}