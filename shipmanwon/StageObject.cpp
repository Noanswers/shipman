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

void CStageObject::createStage()
{
	Verticies.clear();
	Indices.clear();

//	up
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.5f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });

//	front
	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f,  0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });

//	left
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f,0.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });

//	back
	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f,0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,4.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f, 0.0f, 4.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });

//	right
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(6.0f,-20.0f,0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });

//	bottom
	Verticies.push_back({ DirectX::XMFLOAT3( 6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });
	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f,-20.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.5f) });
	
	//!! temp!! 수정이 시급함! 더 안전하게 만들어야 함!
	unsigned long a[] = {
		// top
		2, 3, 0, 2, 0, 1,
		// front
		6, 7, 4, 6, 4, 5,
		// bottom
		10, 11, 8, 10, 8, 9,
		// back
		14, 15, 12, 14, 12, 13,
		// right
		18, 19, 16, 18, 16, 17,
		// left
		22, 23, 20, 22, 20, 21
	}; 
	
	Indices.insert(Indices.end(), a, a + 36);

	realPosition[0] = DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f);
	realPosition[1] = DirectX::XMFLOAT3(0.0f, 0.0f, 4.0f);
	realPosition[2] = DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f);
	realPosition[3] = DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f);
}

bool CStageObject::isGetOutStage(DirectX::XMFLOAT3 pos)
{
	std::unordered_set<float> temp;
	
	float x = pos.x;
	float z = pos.z;
	float x1, x2, z1, z2;

	for (int i = 0; i < 4 /*vertix_num*/; i++)
	{
		x1 = realPosition[i].x;
		z1 = realPosition[i].z;
		x2 = realPosition[(i + 1) % 4].x;
		z2 = realPosition[(i + 1) % 4].z;

		float result = (z - z1) *((x2 - x1) / (z2 - z1)) + x1;

		if (result < x)
			continue;
		/*if (result >= x)*/
		if (result <= max(x1, x2) && result >= min(x1, x2))
			temp.insert(temp.end(), result);
	}

	if (temp.size() % 2 == 0)
		return true;

	return false;
}