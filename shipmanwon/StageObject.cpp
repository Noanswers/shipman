#include "stdafx.h"
#include <unordered_set>
#include "StageObject.h"
#include "config.h"

bool CStageObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (textureFilename.empty() == true)
		textureFilename = textureStage;

	if (IsInit == true)
		return true;

	pTemp_Device = device;

	loadTexture();
	createShader();
	createStage(topRadius, bottomRadius, height, slice);

	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	return result;
}

void CStageObject::shutdown()
{
	CMyObject::shutdown();
	shutdownBuffers();

	return;
}

void CStageObject::createShader()
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

	hr = pTemp_Device->CreateInputLayout(
		layout, numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&pVertexLayout);

	pVSBlob->Release();

	ID3DBlob* pPSBlob = NULL;

	hr = D3DCompileFromFile(
		StageFxFile.c_str(), 0, 0,			// Shader 파일 설정
		"PS", "ps_5_0",											// 컴파일 설정
		0, 0,												// 쉐이더 옵션
		&pPSBlob, &pErrorBlob);								// 리턴

	hr = pTemp_Device->CreatePixelShader(
		pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		0, &pPixelShader);

	pPSBlob->Release();
}

void CStageObject::createStage(float topRadius, float bottomRadius, float height, int slice)
{
	Verticies.clear();
	Indices.clear();

	createTempVertex(topRadius, bottomRadius, height, slice, stack);
	createUpperTriangleOnStack(slice);
	createLowerTriangleOnStack(slice);
}

void CStageObject::createTempVertex(float topRadius, float bottomRadius, float height, int slice, int stack)
{
	float delta = bottomRadius - topRadius;

	//	Top Bottom 생성
	float theta = XM_2PI / slice;
	Verticies.push_back({
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.8f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT2(0.5f, 0.0f) }
	);

	for (int i = 0; i <= stack; ++i)
	{
		float a = XM_PI / slice * (i % 2);
		float stackHeight = height / stack * i;
		for (int j = 0; j <= slice; ++j)
		{
			int idx = j%slice;
			float radius = topRadius + delta*i;

			VertexType vertex = {
				XMFLOAT3(radius*cosf(theta*idx - a), -stackHeight, radius*sinf(theta*idx - a)),
				XMFLOAT4(0.8f, 1.0f, 1.0f, 1.0f),
				XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT2(1.0f, 0.0f)
			};
			Verticies.push_back(vertex);
			BorderVertices.push_back(vertex.position);
		}
	}
	Verticies.push_back({
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.8f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(0.0f, -1.0f, 0.0f),
		XMFLOAT2(0.5f, 0.0f) }
	);
	//	Indices 추가
	for (int i = 0; i < slice; ++i)
	{
		Indices.push_back(0);
		Indices.push_back(i + 2);
		Indices.push_back(i + 1);
	}
}

void CStageObject::createUpperTriangleOnStack(int slice)
{
	//	윗 삼각형을 구성하는 정점 생성
	for (int i = 0; i < slice; ++i)
	{
		XMFLOAT3 pos1 = Verticies[i + 1].position;
		XMFLOAT3 pos2 = Verticies[i + 2].position;
		XMFLOAT3 pos3 = Verticies[i + 2 + slice + 1].position;

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

		auto newVertex1 = Verticies[i + 1];
		auto newVertex2 = Verticies[i + 2];
		auto newVertex3 = Verticies[i + 2 + slice + 1];

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

void CStageObject::createLowerTriangleOnStack(int slice)
{
	//	아래 삼각형을 구성하는 정점 생성
	for (int i = 0; i < slice; ++i)
	{
		XMFLOAT3 pos1 = Verticies[i + 1].position;
		XMFLOAT3 pos2 = Verticies[i + 2 + slice + 1].position;
		XMFLOAT3 pos3 = Verticies[i + 2 + slice].position;

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

		auto newVertex1 = Verticies[i + 1];
		auto newVertex2 = Verticies[i + 2 + slice + 1];
		auto newVertex3 = Verticies[i + 2 + slice];

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

bool CStageObject::isGetOutStage(DirectX::XMFLOAT3 pos)
{
	std::unordered_set<float> temp;

	float x = pos.x;
	float z = pos.z;
	float x1, x2, z1, z2;

	for (int i = 1; i <= slice; ++i)
	{
		x1 = BorderVertices[i].x;
		z1 = BorderVertices[i].z;
		x2 = BorderVertices[(i + 1) % slice].x;
		z2 = BorderVertices[(i + 1) % slice].z;

		float result = (z - z1) *((x2 - x1) / (z2 - z1)) + x1;

		if (result < x)
			continue;

		if (result <= max(x1, x2) && result >= min(x1, x2))
			temp.insert(temp.end(), result);
	}

	if (temp.size() % 2 == 0)
		return true;

	return false;
}