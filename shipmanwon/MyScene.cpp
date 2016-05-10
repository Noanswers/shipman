#include "stdafx.h"
#include "MyScene.h"

bool CMyScene::renderScene(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result = false;
	for (auto& iter : LayerVector)
	{
		result = iter->renderLayer(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	}
	return result;
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

bool CMyScene::initScene(ID3D11Device* device, HWND hWnd)
{
	for (auto& iter : LayerVector)
	{
		bool result = iter->initLayer(device, hWnd);
		if (result == false)
			return false;
	}
	return true;
}