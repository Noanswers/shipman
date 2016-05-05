#include "stdafx.h"
#include "MyScene.h"

void CMyScene::renderScene(ID3D11DeviceContext* deviceContext)
{
	for (auto& iter : LayerVector)
	{
		iter->renderLayer(deviceContext);
	}
}

bool CMyScene::pushBack(CMyLayer* layer)
{
	size_t preSize = LayerVector.size();
	LayerVector.push_back(layer);

	size_t currentSize = LayerVector.size();
	if (currentSize == (preSize + 1))
		return true;

	return false;
}

bool CMyScene::initScene(ID3D11Device* device)
{
	for (auto& iter : LayerVector)
	{
		bool result = iter->initLayer(device);
		if (result == false)
			return false;
	}
	return true;
}