#include "stdafx.h"
#include "MyLayer.h"

void CMyLayer::renderLayer(ID3D11DeviceContext* deviceContext)
{
	for (auto& iter : ObjectVector)
	{
		iter->renderObject(deviceContext);
	}
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

bool CMyLayer::initLayer(ID3D11Device* device)
{
	for (auto& iter : ObjectVector)
	{
		bool result = iter->initialize(device);
		if (result == false)
			return false;
	}
	return true;
}