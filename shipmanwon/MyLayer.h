#pragma once

#include <d3d11.h>
#include <vector>
#include "MyObject.h"

class CMyLayer
{
public:
	CMyLayer() = default;
	CMyLayer(const CMyLayer&) = default;
	~CMyLayer() = default;

	void renderLayer(ID3D11DeviceContext* deviceContext);

private:
	std::vector<CMyObject> ObjectVector;
};

