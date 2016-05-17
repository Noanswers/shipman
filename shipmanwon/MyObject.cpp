#include "stdafx.h"
#include "MyObject.h"
#include "config.h"

bool CMyObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (textureFilename.empty() == true)
		textureFilename = textureDefault;
	
	if (IsInit == true)
		return true;

	//bool result = initializeBuffers(device);
	bool result = initializeBuffers(device, vertices, indices, m_vertexCount, m_indexCount);
	if (result == true)
		IsInit = true;

	loadTexture();
	createShader();

	return result;
}

void CMyObject::shutdown()  
{
	// Shutdown the vertex and index buffers.
	shutdownBuffers();

	return;
}

bool CMyObject::renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc)
{
	bool result = true;
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	
	setShaderfunc(deviceContext, this);
	renderBuffers(deviceContext);
	
	return result;
}

int CMyObject::getIndexCount()
{
	return m_indexCount;
}

void CMyObject::moveToward(float x, float y, float z)
{
	float sum = x + y + z;
	DirectX::XMMATRIX temp = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		x, y, z, 0.0f
	};
	ObjectTranslate += ((temp/sum) * speed);

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::moveForward()
{
	float vecX, vecY, vecZ;
	DirectX::XMVector3Normalize(frontVector);

	DirectX::XMVectorGetByIndexPtr(&vecX, frontVector, 0);
	DirectX::XMVectorGetByIndexPtr(&vecY, frontVector, 1);
	DirectX::XMVectorGetByIndexPtr(&vecZ, frontVector, 2);
	
	moveToward(vecX, vecY, vecZ);
}

DirectX::XMMATRIX CMyObject::getWorldMatrix()
{
	return ObjectWorld;
}

void CMyObject::setTranslate(float x, float y, float z)
{
	ObjectTranslate = DirectX::XMMatrixTranslation(x, y, z);

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::setRotate(float x, float y, float z)
{
	x /= DirectX::XM_2PI;
	y /= DirectX::XM_2PI;
	z /= DirectX::XM_2PI;

	/*float curCos = cosf(ForwardTheta.y);
	float curSin = sinf(ForwardTheta.y);*/
	
	/*DirectX::XMMATRIX mat = {
		ForwardVector.x, 0.0f, 0.0f, 0.0f,
		0.0f, ForwardVector.y, 0.0f, 0.0f,
		0.0f, 0.0f, ForwardVector.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat *= DirectX::XMMatrixRotationX(x);
	mat *= DirectX::XMMatrixRotationY(y);
	mat *= DirectX::XMMatrixRotationZ(z);*/

	//frontVector = DirectX::XMVectorRotateRight(frontVector, 0);
	DirectX::XMVECTOR rotate = DirectX::XMQuaternionIdentity();
	frontVector = DirectX::XMVector3Rotate(frontVector, rotate);
	/*DirectX::XMVectorGetByIndexPtr(&ForwardVector.x, mat.r[0], 0);
	DirectX::XMVectorGetByIndexPtr(&ForwardVector.y, mat.r[0], 1);
	DirectX::XMVectorGetByIndexPtr(&ForwardVector.z, mat.r[0], 2);*/

	//ForwardVector.x = mat.r[0]
	/*ForwardVector.x = curCos*cosf(y) - curSin*sinf(y);
	ForwardVector.z = curSin*cosf(y) + curCos*sinf(y);*/

	//ForwardTheta.y += y;

	DirectX::XMMATRIX temp = DirectX::XMMatrixIdentity();
	temp = DirectX::XMMatrixRotationX(x);
	temp *= DirectX::XMMatrixRotationY(y);
	temp *= DirectX::XMMatrixRotationZ(z);

	ObjectRotate *= temp;

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::setScale(float x, float y, float z)
{
	ObjectScale = DirectX::XMMatrixScaling(x, y, z);

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::setColorRGBA(float red, float green, float blue, float alpha)
{
	for (int i = 0; i < m_vertexCount; ++i)
	{
		vertices[i].color = DirectX::XMFLOAT4(red, green, blue, alpha);
	}
	
	// �ӽ� �ڵ�
	if (temp_device == nullptr)
		return;

	initializeBuffers(temp_device, vertices, indices, m_vertexCount, m_indexCount);
}


/*
	=== [ private ] ===========================================================================
*/

HRESULT CMyObject::loadTexture()
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(temp_device, textureFilename.c_str(), &Resource, &m_pTextureRV, NULL);

	/*HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		temp_device, L"Texture/images.jpg", NULL, NULL, &g_pTextureRV, NULL);*/

	//�ؽ��ĸ� ������ D3D device, �ؽ��� ���, �߰� �̹��� ����(���� NULL), �ڿ� ���� ������ ( ���� NULL) ������ �̹��� ��, �ڿ� ���� �����尡 NULL �̸� NULL

	if (FAILED(hr))
		return hr;

	D3D11_SAMPLER_DESC	sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = temp_device->CreateSamplerState(&sampDesc, &m_pSamplerLinear);	// SamplerState ����
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void CMyObject::createShader()
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pVSBlob = NULL;
	HRESULT hr = D3DCompileFromFile(
		FxFile.c_str(), 0, 0,
		"VS", "vs_5_0",
		0, 0,
		&pVSBlob, &pErrorBlob);

	//���ؽ� ���̴� ����
	hr = temp_device->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		0, &m_pVertexShader);

	//	�ش� D3D_INPUT_ELEMENT_DESC �� vertex�� struct�� ���缭 �����ؾ� ��

	D3D11_INPUT_ELEMENT_DESC	layout[] =
	{
		//4��° ���ڴ� ���� index
		//D3D11_APPEND_ALIGHNED_ELEMENT�� ��� ���� offset �ڵ� ���

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }

		//	D3D11_APPEND_ALIGNED_ELEMENT ���� offset �ڵ� ����� �˴ϴ�.
	};

	UINT numElements = ARRAYSIZE(layout);

	hr = temp_device->CreateInputLayout(layout, numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&m_pVertexLayout);

	pVSBlob->Release();

	ID3DBlob* pPSBlob = NULL;

	hr = D3DCompileFromFile(FxFile.c_str(), 0, 0,			// Shader ���� ����
		"PS", "ps_5_0",											// ������ ����
		0, 0,												// ���̴� �ɼ�
		&pPSBlob, &pErrorBlob);								// ����

	hr = temp_device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		0, &m_pPixelShader);

	pPSBlob->Release();
}

//bool CMyObject::initializeBuffers(ID3D11Device* device)
bool CMyObject::initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices, int m_vertexCount, int m_indexCount)
{
	temp_device = device;

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;

	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.

	return true;
}

void CMyObject::shutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void CMyObject::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType);;
	unsigned int offset = 0;

	deviceContext->IASetInputLayout(m_pVertexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//�ؽ���!
	deviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	deviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

	// ������ ������ �������� ������ �������������� �����Ϸ��� ���۸� ��ġ�� �� �Է� ���Կ� ����� ��
	// 1��° ���� : 
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.

	deviceContext->DrawIndexed(m_indexCount, 0, 0);

	return;
}

void CMyObject::update()
{

}