#include "stdafx.h"
#include "MyLayer.h"

void CMyLayer::renderLayer(ID3D11DeviceContext* deviceContext)
{
	for (auto& iter : ObjectVector)
	{
		iter.renderObject(deviceContext);
	}
}
