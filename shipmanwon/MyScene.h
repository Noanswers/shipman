#pragma once

#include <d3d11.h>
#include <tuple>
#include <list>
#include "MyObject.h"
#include "MyLayer.h"

class CMyScene
{
public:
	CMyScene() = default;
	CMyScene(const CMyScene&) = delete;
	virtual ~CMyScene() = default;

	virtual void initialize();
	bool initScene(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyObject* object, int layerNum);
	bool renderScene(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

	void getSceneColor(float colorSet[4]) const;
	void setSceneColor(const float red, const float green, const float blue, const float alpha);

private:
	std::list<std::tuple<int, CMyLayer*>> LayerList;
	bool checkLayerExist(int layerNum);
	CMyLayer* getLayerByNum(int layerNum);

	float SceneColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
};