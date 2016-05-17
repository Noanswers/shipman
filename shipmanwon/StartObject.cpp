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
	stVerticies.clear();
	stIndices.clear();



}
HRESULT CStartObject::loadTexture()
{


}

void CStartObject::createShader()
{

}

void CStartObject::createShader()
{

}

void CStartObject::renderBuffers(ID3D11DeviceContext* deviceContext)
{

}

bool CStartObject::initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices, int m_vertexCount, int m_indexCount)
{

}