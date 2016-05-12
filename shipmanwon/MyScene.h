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

		CMyObject* object2 = new CMyObject();
		pushBack(object2, 9);

		CMyObject* object3 = new CMyObject();
		pushBack(object3, 10);

		CMyObject* object4 = new CMyObject();
		pushBack(object4, 10);

		object1->setTranslate(3.0f, 0.0f, 0.0f);
		object1->setScale(0.5f, 0.5f, 0.5f);

		object2->setTranslate(-3.0f, 0.0f, 0.0f);
		object2->setScale(0.5f, 0.5f, 0.5f);

		object3->setTranslate(0.0f, 2.0f, 0.0f);
		object3->setScale(0.5f, 0.5f, 0.5f);
		object3->setColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);

		object4->setTranslate(0.0f, -2.0f, 0.0f);
		object4->setScale(0.5f, 0.5f, 0.5f);
		object4->setColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);
	};
	CMyScene(const CMyScene&) = delete;
	virtual ~CMyScene() = default;

	bool initScene(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyObject* object, int layerNum);
	bool renderScene(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	std::list<std::tuple<int, CMyLayer*>> LayerList;
	bool checkLayerExist(int layerNum);
	CMyLayer* getLayerByNum(int layerNum);
};