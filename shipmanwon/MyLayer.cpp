#include "stdafx.h"
#include "MyLayer.h"

bool CMyLayer::renderLayer(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	bool result = false;
	for (auto& iter : ObjectVector)
	{
		result = iter->renderObject(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	
		if (!result)
			return result;
	}

	return result;
}

bool CMyLayer::pushBack(CMyObject* object)
{
	size_t preSize = ObjectVector.size();
	ObjectVector.push_back(object);

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