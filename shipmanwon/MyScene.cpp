#include "stdafx.h"
#include "MyScene.h"

void CMyScene::renderScene(ID3D11DeviceContext* deviceContext)
{
	for (auto& iter : LayerVector)
	{
		iter.renderLayer(deviceContext);
	}
}