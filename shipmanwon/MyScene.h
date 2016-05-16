#pragma once

#include <d3d11.h>
#include <tuple>
#include <list>
#include <functional>
#include "MyObject.h"
#include "MyLayer.h"

class CMyScene
{
public:
	CMyScene() = default;
	CMyScene(const CMyScene&) = delete;
	virtual ~CMyScene() = default;

	virtual void initialize() = 0;
	bool initScene(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyObject* object, int layerNum);
	//bool renderScene(ID3D11DeviceContext* deviceContext);
	bool renderScene(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc);

	void getSceneColor(float colorSet[4]) const;
	void setSceneColor(const float red, const float green, const float blue, const float alpha);
	
	void update();

private:
	std::list<std::tuple<int, CMyLayer*>> LayerList;
	bool checkLayerExist(int layerNum);
	CMyLayer* getLayerByNum(int layerNum);

	float SceneColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
};