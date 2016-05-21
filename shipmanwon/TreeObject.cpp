#include "stdafx.h"
#include "TreeObject.h"
#include "config.h"

bool CTreeObject::initialize(ID3D11Device* device, HWND hWnd)
{
	temp_device = device;

	if (textureFilename.empty() == true)
		textureFilename = textureStage;

	if (IsInit == true)
		return true;
	
	loadTexture();
	createShader();
	createTree(upperHeight, upperWidth, lowerWidthTop, lowerWidthBottom, lowerHeight, slice);
	
	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;
	
	return result;
}

void CTreeObject::shutdown()
{
	CMyObject::shutdown();
	shutdownBuffers();

	return;
}

void CTreeObject::createShader()
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pVSBlob = NULL;
	HRESULT hr = D3DCompileFromFile(
		StageFxFile.c_str(), 0, 0,
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

	hr = D3DCompileFromFile(
		StageFxFile.c_str(), 0, 0,			// Shader 파일 설정
		"PS", "ps_5_0",											// 컴파일 설정
		0, 0,												// 쉐이더 옵션
		&pPSBlob, &pErrorBlob);								// 리턴

	hr = temp_device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		0, &m_pPixelShader);

	pPSBlob->Release();
}

void CTreeObject::createTree(float upperHeight, float upperWidth, float lowerWidthTop, float lowerWidthBottom, float lowerHeight, int slice)
{
	/*float totalHeight = upperHeight + lowerHeight;
	float delta = lowerWidthBottom - lowerWidthTop;
*/
	Verticies.clear();
	Indices.clear();

	createTempUpperVertex(upperHeight, upperWidth, lowerHeight, slice);
	createUpperPart(upperHeight, upperWidth, lowerHeight, slice);

	int base = Verticies.size();
	
	createTempVertexOfWood(lowerWidthTop, lowerWidthBottom, lowerHeight, slice);
	createUpperTriangleOfWood(lowerWidthTop, lowerWidthBottom, slice, base);
	createLowerTriangleOfWood(lowerWidthTop, lowerWidthBottom, slice, base);
}

void CTreeObject::createTempUpperVertex(float upperHeight, float upperwidth, float lowerHeight, int slice)
{
	float totalHeight = upperHeight + lowerHeight;

	//	upper 생성 (나뭇잎 부분)
	Verticies.push_back({
		XMFLOAT3(0.0f, totalHeight, 0.0f),
		XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT2(0.0f, 0.0f) }
	);

	float theta = XM_2PI / slice;

	for (int i = 0; i <= slice; ++i)
	{
		float radius = upperwidth / 2;
		Verticies.push_back({
			XMFLOAT3(radius*cosf(theta*i), lowerHeight, radius*sinf(theta*i)),
			XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f),
			XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT2(0.0f, 0.0f) }
		);
	}
}

void CTreeObject::createUpperPart(float upperHeight, float upperwidth, float lowerHeight, int slice)
{
	//float totalHeight = upperHeight + lowerHeight;

	////	upper 생성 (나뭇잎 부분)
	//Verticies.push_back({
	//	XMFLOAT3(0.0f, totalHeight, 0.0f),
	//	XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f),
	//	XMFLOAT3(0.0f, 1.0f, 0.0f),
	//	XMFLOAT2(0.0f, 0.0f) }
	//);

	//float theta = XM_2PI / slice;

	//for (int i = 0; i <= slice; ++i)
	//{
	//	float radius = upperwidth / 2;
	//	Verticies.push_back({
	//		XMFLOAT3(radius*cosf(theta*i), lowerHeight, radius*sinf(theta*i)),
	//		XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f),
	//		XMFLOAT3(0.0f, 1.0f, 0.0f),
	//		XMFLOAT2(0.0f, 0.0f) }
	//	);
	//}

	// flat design을 위해 normal vector를 계산하여 새로 생성
	for (int i = 0; i < slice; ++i)
	{
		XMFLOAT3 pos1 = Verticies[0].position;
		XMFLOAT3 pos2 = Verticies[i + 2].position;
		XMFLOAT3 pos3 = Verticies[i + 1].position;

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

		auto newVertex1 = Verticies[0];
		auto newVertex2 = Verticies[i + 2];
		auto newVertex3 = Verticies[i + 1];

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

void CTreeObject::createTempVertexOfWood(float lowerWidthTop, float lowerWidthBottom, float lowerHeight, int slice)
{
	float delta = lowerWidthBottom - lowerWidthTop;
	float theta = XM_2PI / slice;

	// lower part 생성 (목재)
	for (int i = 0; i < 2; ++i)
	{
		float radius = lowerWidthTop / 2 + delta*i;
		for (int j = 0; j <= slice; ++j)
		{
			Verticies.push_back({
				XMFLOAT3(radius*cosf(theta*j), lowerHeight - lowerHeight*i, radius*sinf(theta*j)),
				XMFLOAT4(0.4f, 0.2f, 0.0f, 1.0f),
				XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT2(0.0f, 0.0f) }
			);
		}
	}
}

void CTreeObject::createUpperTriangleOfWood(float lowerWidthTop, float lowerWidthBottom, int slice, int base)
{
	float delta = lowerWidthBottom - lowerWidthTop;
	float theta = XM_2PI / slice;

	//	아래 목재부분을 구성하는 정점 새로 생성
	for (int i = 0; i < slice; ++i)
	{
		XMFLOAT3 pos1 = Verticies[base + i].position;
		XMFLOAT3 pos2 = Verticies[base + i + 1].position;
		XMFLOAT3 pos3 = Verticies[base + i + slice + 2].position;

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

		auto newVertex1 = Verticies[base + i];
		auto newVertex2 = Verticies[base + i + 1];
		auto newVertex3 = Verticies[base + i + slice + 2];

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

void CTreeObject::createLowerTriangleOfWood(float lowerWidthTop, float lowerWidthBottom, int slice, int base)
{
	float delta = lowerWidthBottom - lowerWidthTop;
	float theta = XM_2PI / slice;

	for (int i = 0; i < slice; ++i)
	{
		XMFLOAT3 pos1 = Verticies[base + i].position;
		XMFLOAT3 pos2 = Verticies[base + i + slice + 2].position;
		XMFLOAT3 pos3 = Verticies[base + i + slice + 1].position;

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

		auto newVertex1 = Verticies[base + i];
		auto newVertex2 = Verticies[base + i + slice + 2];
		auto newVertex3 = Verticies[base + i + slice + 1];

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