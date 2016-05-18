#include "stdafx.h"
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
}

bool CStageObject::isGetOutStage(DirectX::XMFLOAT3 position)
{
	int count = 0;
	int i;
	double xinters;
	int xq=0;
	DirectX::XMFLOAT3 temp1 = Verticies[0].position;
	for (i = 1; i <= 4; i++)
	{
		DirectX::XMFLOAT3 temp2 = Verticies[i % 4].position;
		if(position.z > min(temp1.z, temp2.z))
			if(position.z <= max(temp1.z, temp2.z))
				if (position.x <= max(temp1.x, temp2.x))
				{
					if (temp1.z != temp2.z)
					{
						xq = (position.z - temp1.z) * (temp2.x - temp1.x) / (temp2.z - temp1.z) + temp1.x;
						if (temp1.x == temp2.x || position.x <= xq)
							count++;
					}
				}
		temp1 = temp2;
	}

	if (count % 2 == 0)return false;

	return true;
}