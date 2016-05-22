#include "stdafx.h"
#include "ResultObject.h"
#include "config.h"
#include <string>





bool CResultObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (temp_device != device)
	{
		temp_device = device;
		IsInit = false;
	}



	if (textureFilename.empty() == true)
	{
		//char *playernum = 
		textureFilename = resultTextureDefault;
		textureFilename += std::to_wstring(winPlayerNum);
		textureFilename += L".jpg";

	}
		

	if (IsInit == true)
		return true;

	createObject();
	loadTexture();
	createShader();

	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	return result;
}


void CResultObject::shutdown()
{
	shutdownBuffers();

	return;
}

void CResultObject::createObject()
{
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 0.0f, 3.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 0.0f, -3.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f, 0.0f, -3.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f, 0.0f, 3.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 3.0f, -3.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,3.0f, -3.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f, 3.0f, 3.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });

	//!! temp!! 수정이 시급함! 더 안전하게 만들어야 함!
	unsigned long a[] = {
		// top
		6, 7, 5,
		7, 4, 5,

		// front
		2, 6, 1,
		6, 5, 1,

		// right
		7, 6, 3,
		3, 6, 2,

		// left
		1, 5, 0,
		0, 5, 4,

		// back
		0, 4, 7,
		0, 7, 3,

		// bottom
		1, 0, 2,
		0, 3, 2
	};
	Indices.insert(Indices.end(), a, a + 36);
}