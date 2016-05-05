#pragma once

#include <d3d11.h>
#include "MyObject.h"
#include "MyLayer.h"

class CMyScene
{
public:
	CMyScene() = default;
	CMyScene(const CMyScene&) = delete;
	~CMyScene() = default;

	void renderScene(ID3D11DeviceContext* deviceContext);

private:
	std::vector<CMyLayer> LayerVector;
};

