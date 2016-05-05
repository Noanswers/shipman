#pragma once

#include <d3d11.h>
#include <vector>
#include "MyObject.h"

class CMyLayer
{
public:
	CMyLayer()
	{
		CMyObject* tmpObject = new CMyObject();
		ObjectVector.push_back(tmpObject);
		//tmpObject->initialize();
	}
	CMyLayer(const CMyLayer&) = default;
	~CMyLayer()
	{

	}

	bool initLayer(ID3D11Device* device);

	bool pushBack(CMyObject* object);
	void renderLayer(ID3D11DeviceContext* deviceContext);

private:
	std::vector<CMyObject*> ObjectVector;
};

