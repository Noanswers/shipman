#pragma once

#include <d3d11.h>
#include <tuple>
#include <list>
#include "MyObject.h"
#include "MyLayer.h"

class CMyScene
{
public:
	CMyScene() 
	{
		CMyObject* object1 = new CMyObject();
		pushBack(object1, 10);

		object1->setTranslate(1.0f, 0.0f, 0.0f);
		object1->setRotate(0.0f, 0.0f, 0.1f);
		object1->setScale(0.8f, 1.0f, 1.0f);
	};
	CMyScene(const CMyScene&) = delete;
	~CMyScene() = default;

	bool initScene(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyObject* object, int layerNum);
	bool renderScene(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	std::list<std::tuple<int, CMyLayer*>> LayerList;
	bool checkLayerExist(int layerNum);
	CMyLayer* getLayerByNum(int layerNum);
};