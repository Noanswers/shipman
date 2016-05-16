#include "stdafx.h"
#include "MyScene.h"

void CMyScene::initialize()
{

}

bool CMyScene::renderScene(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc)
{
	for (auto& iter : LayerList)
	{
		CMyLayer* layer = std::get<CMyLayer*>(iter);
		bool result = layer->renderLayer(deviceContext, setShaderfunc);
	
		if (!result)
			return result;
	}
}


void CMyScene::update()
{
	for (auto& iter : LayerList)
	{
		CMyLayer* layer = std::get<CMyLayer*>(iter);
		layer->update();

	}
}

void CMyScene::getSceneColor(float colorSet[4]) const
{
	for (int i = 0; i < 4; ++i)
	{
		colorSet[i] = SceneColor[i];
	}
}

void CMyScene::setSceneColor(const float red, const float green, const float blue, const float alpha)
{
	SceneColor[0] = red;
	SceneColor[1] = green;
	SceneColor[2] = blue;
	SceneColor[3] = alpha;
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