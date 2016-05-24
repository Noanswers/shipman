#include "stdafx.h"
#include "MyLayer.h"

bool CMyLayer::renderLayer(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc)
{
	for (auto& iter : ObjectVector)
	{
		bool result = iter->renderObject(deviceContext, setShaderfunc);
	
		if (!result)
			return result;
	}
}

bool CMyLayer::pushBack(CMyObject* object)
{
	CMyObject* obj = object;
	size_t preSize = ObjectVector.size();
	ObjectVector.push_back(obj);

	size_t currentSize = ObjectVector.size();
	if (currentSize == (preSize + 1))
		return true;

	return false;
}

bool CMyLayer::initLayer(ID3D11Device* device, HWND hWnd)
{
	for (auto& iter : ObjectVector)
	{
		bool result = iter->initialize(device, hWnd);
		if (result == false)
			return false;
	}
	return true;
}

int CMyLayer::getObjectsSize()
{
	return ObjectVector.size();
}