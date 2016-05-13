#include "stdafx.h"
#include "StageObject.h"

bool CStageObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (IsInit == true)
		return true;

	//bool result = initializeBuffers(device);
	bool result = initializeBuffers(device, vertices, indices, m_vertexCount, m_indexCount);
	if (result == true)
		IsInit = true;

	ObjectShader = new CColorShaderClass();
	result = ObjectShader->Initialize(device, hWnd);

	return result;
}

void CStageObject::shutdown()
{
	CMyObject::shutdown();

	return;
}

bool CStageObject::renderObject(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result = false;
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(deviceContext);

	result = ObjectShader->Render(deviceContext, m_indexCount, ObjectWorld, viewMatrix, projectionMatrix);

	return result;
}


void CStageObject::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

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