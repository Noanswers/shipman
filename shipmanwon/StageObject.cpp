#include "stdafx.h"
#include <unordered_set>
#include "StageObject.h"
#include "config.h"

bool CStageObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (textureFilename.empty() == true)
		textureFilename = texturestage;

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
	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });

	Indices.push_back(0);
	Indices.push_back(1);
	Indices.push_back(2);
	Indices.push_back(2);
	Indices.push_back(1);
	Indices.push_back(3);

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
	int count = 0;

	for (int i = 0; i < 4 /*vertix_num*/; i++)
	{
		x1 = realPosition[i].x;
		x2 = realPosition[(i + 1) % 4].x;
		z1 = realPosition[i].z;
		z2 = realPosition[(i + 1) % 4].z;

		float result = (z - z1) *((x2 - x1) / (z2 - z1)) + x1;

		if (result >= x)
			if (result <= max(x1, x2))
				temp.insert(temp.end(), result);
	}

	if (temp.size() % 2 == 0)
		return true;

	return false;
}