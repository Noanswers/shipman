#pragma once
#include <d3d11.h>

class CMyObject
{
public:
	CMyObject() = default;
	CMyObject(const CMyObject&) = default;
	~CMyObject() = default;

	void renderObject(ID3D11DeviceContext* deviceContext);

private:

};

