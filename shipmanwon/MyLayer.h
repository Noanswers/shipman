#pragma once

#include <d3d11.h>
#include <vector>
#include "MyObject.h"

class CMyLayer
{
public:
	CMyLayer()
	{
		CMyObject* tmpObject = new CMyObject();
		ObjectVector.push_back(tmpObject);
	}
	CMyLayer(const CMyLayer&) = default;
	~CMyLayer()
	{

	}

	bool initLayer(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyObject* object);
	bool renderLayer(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	std::vector<CMyObject*> ObjectVector;
};

