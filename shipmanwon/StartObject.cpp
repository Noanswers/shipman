#include "stdafx.h"
#include "StartObject.h"


CStartObject::CStartObject()
{
}


CStartObject::~CStartObject()
{
}

bool CStartObject::initialize(ID3D11Device* device, HWND hWnd)
{

	if (temp_device != device)
	{
		temp_device = device;
		IsInit = false;
	}


	if (textureFilename.empty() == true)
		textureFilename = textureDefault;

	if (IsInit == true)
		return true;

	loadTexture();
	createShader();
	createTitleBox();

	bool result = initializeBuffers(device, vertices, indices, m_vertexCount, m_indexCount);
	if (result == true)
		IsInit = true;

	return result;
}

void CStartObject::shutdown()
{


}

bool CStartObject::renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc)
{

}

void CStartObject::createTitleBox()
{

}