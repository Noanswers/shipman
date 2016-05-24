#pragma once

#include <d3d11.h>
#include <tuple>
#include <list>
#include <vector>
#include <functional>
#include "MyObject.h"
#include "MyLayer.h"

class CMyScene
{
public:
	CMyScene() = default;
	CMyScene(const CMyScene&) = delete;
	virtual ~CMyScene()
	{
		for (auto& iter : LayerList)
		{
			CMyLayer* curlayer = std::get<CMyLayer*>(iter);
			delete curlayer;
		}
		LayerList.clear();
		ObjectList.clear();
	};

	virtual void initialize() = 0;
	bool initScene(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyObject* object, int layerNum);
	bool renderScene(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc);

	void getSceneColor(float colorSet[4]) const;
	void setSceneColor(const float red, const float green, const float blue, const float alpha);

	std::vector<CMyObject*> getObjectsByName(std::string objName);
	virtual void doAction(float delta);

protected:
	std::list<std::tuple<int, CMyLayer*>> LayerList;
	std::list<CMyObject*> ObjectList;

	bool checkLayerExist(int layerNum);
	CMyLayer* getLayerByNum(int layerNum);

	float SceneColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
};