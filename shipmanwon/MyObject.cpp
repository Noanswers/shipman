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

	result = ObjectShader->Render(deviceContext, m_indexCount, worldMatrix, viewMatrix, projectionMatrix);
	return result;
}

int CMyObject::getIndexCount()
{
	return m_indexCount;
}

void CMyObject::setPosition(int x, int y, int z)
{
	xPos = x;
	yPos = y;
	zPos = z;
}

void CMyObject::adjustPosition(VertexType* vertices)
{
	for (int i = 0; i < m_indexCount; ++i)
	{
		m_vertices[i].position.x += xPos;
		m_vertices[i].position.y += yPos;
		m_vertices[i].position.z += zPos;
	}
}



/*
	=== [ private ] ===========================================================================
*/

bool CMyObject::initializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;

	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	
	// Set the number of indices in the index array.
	m_vertexCount = 4;
	m_indexCount = 6;

	// Create the vertex array.
	VertexType vertices[4];
	vertices[0] = { DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[1] = { DirectX::XMFLOAT3( 0.0f, 4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[2] = { DirectX::XMFLOAT3( 0.0f, -4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[3] = { DirectX::XMFLOAT3( 6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	
	VertexType* m_vertice = vertices;
	/*VertexType* newVertice = vertices;
	adjustPosition(newVertice);*/

	// Create the index array.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	//vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position =	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].color =		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;


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
	delete[] indices;
	indices = 0;

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

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->DrawIndexed(m_indexCount, 0, 0);

	return;
}