#include "stdafx.h"
#include "MyScene.h"

bool CMyScene::renderScene(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result = false;
	for (auto& iter : LayerList)
	{
		CMyLayer* layer = std::get<CMyLayer*>(iter);
		result = layer->renderLayer(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	}
	return result;
}

bool CMyScene::pushBack(CMyObject* object, int layerNum)
{
	bool isExist = checkLayerExist(layerNum);
	if (isExist == false)
	{
		LayerList.emplace_back(std::make_tuple(layerNum, new CMyLayer()));
	}

	CMyLayer* layer = getLayerByNum(layerNum);
	
	size_t preSize = layer->getObjectsSize();
	layer->pushBack(object);
	size_t currentSize = layer->getObjectsSize();

	if (currentSize == (preSize + 1))
		return true;

	return false;
}

bool CMyScene::initScene(ID3D11Device* device, HWND hWnd)
{
	for (auto& iter : LayerList)
	{
		CMyLayer* layer = std::get<CMyLayer*>(iter);
		
		bool result = layer->initLayer(device, hWnd);
		if (result == false)
			return false;
	}
	return true;
}

bool CMyScene::checkLayerExist(int layerNum)
{
	bool result = false;
	for (auto& layer : LayerList)
	{
		if (layerNum == std::get<0>(layer))
			result = true;
	}
	return result;
}

CMyLayer* CMyScene::getLayerByNum(int layerNum)
{
	CMyLayer* result = nullptr;
	
	for (auto& layer : LayerList)
	{
		if (layerNum == std::get<0>(layer))
			result = std::get<CMyLayer*>(layer);
	}
	return result;
}