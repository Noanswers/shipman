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

	bool initScene(ID3D11Device* device);

	bool pushBack(CMyLayer* layer);
	void renderScene(ID3D11DeviceContext* deviceContext);

private:
	std::vector<CMyLayer*> LayerVector;
};

