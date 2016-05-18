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

	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	return result;
}

void CPlayerObject::shutdown()
{
	CMyObject::shutdown();

	return;
}

bool CPlayerObject::isCollisionPlayer(CPlayerObject* enemy)
{
	DirectX::XMFLOAT3 en_curPos = enemy->currentPosition;

	if ( CalcDistanceTwoPoint(en_curPos, currentPosition) <= (bottomRadius + enemy->getRadious()) )
		return true;

	return false;
}

void CPlayerObject::createCylinder()
{
	Verticies.clear();
	Indices.clear();

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
			Verticies.push_back(vertex);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//make indicies
	UINT ringVertexCount = sliceCount + 1;

	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			Indices.push_back(i*ringVertexCount + j);
			Indices.push_back((i+1)*ringVertexCount + j);
			Indices.push_back((i+1)*ringVertexCount + j+1);

			Indices.push_back(i*ringVertexCount + j);
			Indices.push_back((i+1)*ringVertexCount + j+1);
			Indices.push_back(i*ringVertexCount + j+1);
		}
	}

	createTopCap();
	createBottomCap();
}

void CPlayerObject::createTopCap()
{
	UINT baseIndex = (UINT)Verticies.size();

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

		Verticies.push_back(vertex);
	}

	//center of cap vertex
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, y, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });

	//center of cap index
	UINT centerIndex = (UINT)Verticies.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		Indices.push_back(centerIndex);
		Indices.push_back(baseIndex + i + 1);
		Indices.push_back(baseIndex + i);
	}
}

void CPlayerObject::createBottomCap()
{
	UINT baseIndex = (UINT)Verticies.size();

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

		Verticies.push_back(vertex);

		/*cyVerticies.push_back({ DirectX::XMFLOAT3(x, y, z), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(u, v) });*/
	}

	//center of cap vertex
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, y, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });

	//center of cap index
	UINT centerIndex = (UINT)Verticies.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		Indices.push_back(centerIndex);
		Indices.push_back(baseIndex + i);
		Indices.push_back(baseIndex + i + 1);
	}
}
