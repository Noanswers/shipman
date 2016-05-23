#include "stdafx.h"
#include "GroundObject.h"
#include "config.h"

bool CGroundObject::initialize(ID3D11Device* device, HWND hWnd)
{
	pTemp_Device = device;

	if (textureFilename.empty() == true)
		textureFilename = textureStage;

	if (IsInit == true)
		return true;

	createGround(100.0f, 100.0f, 50);
	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	loadTexture();
	createShader();

	return result;
}

void CGroundObject::shutdown()
{
	CMyObject::shutdown();
	shutdownBuffers();

	return;
}

void CGroundObject::createShader()
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pVSBlob = NULL;
	HRESULT hr = D3DCompileFromFile(
		StageFxFile.c_str(), 0, 0,
		"VS", "vs_5_0",
		0, 0,
		&pVSBlob, &pErrorBlob);

	//버텍스 쉐이더 생성
	hr = pTemp_Device->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		0, &pVertexShader);

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

	hr = pTemp_Device->CreateInputLayout(layout, numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&pVertexLayout);

	pVSBlob->Release();

	ID3DBlob* pPSBlob = NULL;

	hr = D3DCompileFromFile(
		StageFxFile.c_str(), 0, 0,			// Shader 파일 설정
		"PS", "ps_5_0",						// 컴파일 설정
		0, 0,								// 쉐이더 옵션
		&pPSBlob, &pErrorBlob);				// 리턴

	hr = pTemp_Device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		0, &pPixelShader);

	pPSBlob->Release();
}

void CGroundObject::createGround(float width, float length, int slice)
{
	std::vector<VertexType> tempVec;
	Verticies.clear();
	Indices.clear();

	createTempVertex(tempVec, width, length, slice);
	createGroundLeftUpper(tempVec, slice);
	createGroundRightLower(tempVec, slice);
}

void CGroundObject::createTempVertex(std::vector<VertexType>& vec, float width, float length, int slice)
{
	float deltaWidth = width / slice;
	float deltaLength = length / slice;
	for (int i = 0; i <= slice; ++i)
	{
		for (int j = 0; j <= slice; ++j)
		{
			float height = 3.0f * sinf(100.0f / (rand() % 200));
			vec.push_back({
				XMFLOAT3(deltaWidth*j - width / 2, height, deltaLength*i - length / 2),
				XMFLOAT4(0.2f, 0.3f, 0.0f, 1.0f),
				XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT2(0.0f, 0.0f) }
			);
		}
	}
}

void CGroundObject::createGroundLeftUpper(std::vector<VertexType>& tempVec, int slice)
{
	for (int i = 0; i < slice; ++i)
	{
		for (int j = 0; j < slice; ++j)
		{
			XMFLOAT3 pos1 = tempVec[j + i*(slice + 1)].position;
			XMFLOAT3 pos2 = tempVec[j + (i + 1)*(slice + 1)].position;
			XMFLOAT3 pos3 = tempVec[j + 1 + (i + 1)*(slice + 1)].position;

			XMFLOAT3 vec1, vec2;
			vec1.x = pos2.x - pos1.x;
			vec1.y = pos2.y - pos1.y;
			vec1.z = pos2.z - pos1.z;

			vec2.x = pos3.x - pos1.x;
			vec2.y = pos3.y - pos1.y;
			vec2.z = pos3.z - pos1.z;

			XMVECTOR vector1 = XMLoadFloat3(&vec1);
			XMVECTOR vector2 = XMLoadFloat3(&vec2);

			XMVECTOR cross = XMVector3Cross(vector1, vector2);
			XMVECTOR result = XMVector3Normalize(cross);

			XMFLOAT3 newNormal;
			DirectX::XMStoreFloat3(&newNormal, result);

			auto newVertex1 = tempVec[j + i*(slice + 1)];
			auto newVertex2 = tempVec[j + (i + 1)*(slice + 1)];
			auto newVertex3 = tempVec[j + 1 + (i + 1)*(slice + 1)];

			newVertex1.normal = newNormal;
			newVertex2.normal = newNormal;
			newVertex3.normal = newNormal;

			Verticies.push_back(newVertex1);
			Indices.push_back(Verticies.size() - 1);
			Verticies.push_back(newVertex2);
			Indices.push_back(Verticies.size() - 1);
			Verticies.push_back(newVertex3);
			Indices.push_back(Verticies.size() - 1);
		}
	}
}

void CGroundObject::createGroundRightLower(std::vector<VertexType>& tempVec, int slice)
{
	for (int i = 0; i < slice; ++i)
	{
		for (int j = 0; j < slice; ++j)
		{
			XMFLOAT3 pos1 = tempVec[j + i*(slice + 1)].position;
			XMFLOAT3 pos2 = tempVec[j + 1 + (i + 1)*(slice + 1)].position;
			XMFLOAT3 pos3 = tempVec[j + 1 + i*(slice + 1)].position;

			XMFLOAT3 vec1, vec2;
			vec1.x = pos2.x - pos1.x;
			vec1.y = pos2.y - pos1.y;
			vec1.z = pos2.z - pos1.z;

			vec2.x = pos3.x - pos1.x;
			vec2.y = pos3.y - pos1.y;
			vec2.z = pos3.z - pos1.z;

			XMVECTOR vector1 = XMLoadFloat3(&vec1);
			XMVECTOR vector2 = XMLoadFloat3(&vec2);

			XMVECTOR cross = XMVector3Cross(vector1, vector2);
			XMVECTOR result = XMVector3Normalize(cross);

			XMFLOAT3 newNormal;
			XMStoreFloat3(&newNormal, result);

			auto newVertex1 = tempVec[j + i*(slice + 1)];
			auto newVertex2 = tempVec[j + 1 + (i + 1)*(slice + 1)];
			auto newVertex3 = tempVec[j + 1 + i*(slice + 1)];

			newVertex1.normal = newNormal;
			newVertex2.normal = newNormal;
			newVertex3.normal = newNormal;

			Verticies.push_back(newVertex1);
			Indices.push_back(Verticies.size() - 1);
			Verticies.push_back(newVertex2);
			Indices.push_back(Verticies.size() - 1);
			Verticies.push_back(newVertex3);
			Indices.push_back(Verticies.size() - 1);
		}
	}
}