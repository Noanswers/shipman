#include "stdafx.h"
#include "MyObject.h"

bool CMyObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (IsInit == true)
		return true;

	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	ObjectShader = new CColorShaderClass();
	result = ObjectShader->Initialize(device, hWnd);

	return result;
}

void CMyObject::shutdown()
{
	// Shutdown the vertex and index buffers.
	shutdownBuffers();
	ObjectShader->Shutdown();
	ObjectShader = nullptr;

	return;
}

bool CMyObject::renderObject(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result = false;
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(deviceContext);

	result = ObjectShader->Render(deviceContext, m_indexCount, ObjectWorld , viewMatrix, projectionMatrix);

	return result;
}

int CMyObject::getIndexCount()
{
	return m_indexCount;
}

void CMyObject::setTranslate(float x, float y, float z)
{
	ObjectTranslate = DirectX::XMMatrixTranslation(x, y, z);

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::setRotate(float x, float y, float z)
{
	ObjectRotate = DirectX::XMMatrixRotationX(x);
	ObjectRotate *= DirectX::XMMatrixRotationY(y);
	ObjectRotate *= DirectX::XMMatrixRotationZ(z);

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
	
	// 임시 코드
	if (temp_device == nullptr)
		return;

	initializeBuffers(temp_device);
}

/*
	=== [ private ] ===========================================================================
*/

bool CMyObject::initializeBuffers(ID3D11Device* device)
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
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the texture 
	deviceContext->PSSetShaderResources(0, 1, &m_TextureRV);
	deviceContext->PSSetSamplers(0, 1, &m_SamplerLinear);

	// 생성된 버퍼의 정점들을 실제로 파이프라인으로 공급하려면 버퍼를 장치의 한 입력 슬롯에 묶어야 함
	// 1번째 인자 : 

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->DrawIndexed(m_indexCount, 0, 0);

	return;
}