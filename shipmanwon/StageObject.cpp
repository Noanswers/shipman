#include "stdafx.h"
#include "StageObject.h"

bool CStageObject::initialize(ID3D11Device* device, HWND hWnd)
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

void CStageObject::shutdown()
{
	CMyObject::shutdown();

	return;
}

bool CStageObject::renderObject(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	bool result = false;
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(deviceContext);

	return result;
}


void CStageObject::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType);;
	unsigned int offset = 0;

	deviceContext->IASetInputLayout(m_pVertexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//텍스쳐!
	deviceContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	deviceContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

	// 생성된 버퍼의 정점들을 실제로 파이프라인으로 공급하려면 버퍼를 장치의 한 입력 슬롯에 묶어야 함
	// 1번째 인자 : 
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.

	deviceContext->DrawIndexed(m_indexCount, 0, 0);

	return;
}