#pragma once

#include <d3d11.h>
#include <vector>
#include "MyObject.h"

class CMyLayer
{
public:
	CMyLayer() = default;
	CMyLayer(const CMyLayer&) = default;
	~CMyLayer()
	{

	}

	bool initLayer(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyObject* object);
	bool renderLayer(ID3D11DeviceContext* deviceContext);
	int	getObjectsSize();

private:
	std::vector<CMyObject*> ObjectVector;
};

