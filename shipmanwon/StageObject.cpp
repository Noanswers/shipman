#include "stdafx.h"
#include "StageObject.h"
#include "config.h"

bool CStageObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (textureFilename.empty() == true)
		textureFilename = textureStage;

	if (IsInit == true)
		return true;

	createStage();
	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	loadTexture();
	createShader();

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

void CStageObject::createStage()
{
	float topRadius = 3.0f;
	float bottomRadius = 8.0f;
	float delta = bottomRadius - topRadius;

	float height = 10.0f;
	int slice = 8;
	int stack = 1;

	Verticies.clear();
	Indices.clear();

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
			//theta += sinf(rand());
			//float normal = (-1) ^ i;
			
			Verticies.push_back({ 
				XMFLOAT3(radius*cosf(theta*idx - a), -stackHeight, radius*sinf(theta*idx - a)), 
				XMFLOAT4(0.8f, 1.0f, 1.0f, 1.0f), 
				XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT2(1.0f, 0.0f) }
			);
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
	//for (int i = 0; i < slice; ++i)
	//{
	//	Indices.push_back(Verticies.size()-1);
	//	Indices.push_back(i + 10);
	//	Indices.push_back(i + 11);
	//}

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

		XMVECTOR result = XMVector3Cross(vector1, vector2);
		
		XMFLOAT3 newNormal;
		XMStoreFloat3(&newNormal, result);

		auto newVertex1 = Verticies[i + 1];
		auto newVertex2 = Verticies[i + 2];
		auto newVertex3 = Verticies[i + 2 + slice + 1];

		newVertex1.normal = newNormal;
		newVertex2.normal = newNormal;
		newVertex3.normal = newNormal;

		Verticies.push_back(newVertex1);
		Indices.push_back(Verticies.size() -1);
		Verticies.push_back(newVertex2);
		Indices.push_back(Verticies.size() - 1);
		Verticies.push_back(newVertex3);
		Indices.push_back(Verticies.size() - 1);
	}

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

		XMVECTOR result = XMVector3Cross(vector1, vector2);

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



	//for (int i = 0; i <= stack; ++i)
	//{
	//	int base = Indices.size();
	//	for (int j = 0; j < slice; ++j)
	//	{
	//		Indices.push_back(base + j);
	//		Indices.push_back( + 2);
	//		Indices.push_back(i + 1);
	//	}
	//}

////	up
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.5f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });
//
////	front
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f,  0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//
////	left
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f,0.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//
////	back
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f,0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,4.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f, 0.0f, 4.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//
////	right
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f,-20.0f,0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//
////	bottom
//	Verticies.push_back({ DirectX::XMFLOAT3( 6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });
//	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f,-20.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.5f) });
	
	//!! temp!! 수정이 시급함! 더 안전하게 만들어야 함!
	//unsigned long a[] = {
	//	// top
	//	2, 3, 0, 2, 0, 1,
	//	// front
	//	6, 7, 4, 6, 4, 5,
	//	// bottom
	//	10, 11, 8, 10, 8, 9,
	//	// back
	//	14, 15, 12, 14, 12, 13,
	//	// right
	//	18, 19, 16, 18, 16, 17,
	//	// left
	//	22, 23, 20, 22, 20, 21
	//}; 
	//
	//Indices.insert(Indices.end(), a, a + 36);
}