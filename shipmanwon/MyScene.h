#pragma once

#include <d3d11.h>
#include "MyObject.h"
#include "MyLayer.h"

class CMyScene
{
public:
	CMyScene()
	{
		CMyLayer* startLayer = new CMyLayer();
		LayerVector.push_back(startLayer);
	};
	CMyScene(const CMyScene&) = delete;
	~CMyScene() = default;

	bool initScene(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyLayer* layer);
	bool renderScene(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	std::vector<CMyLayer*> LayerVector;
};

