#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "ColorShaderClass.h"

class CMyObject
{
public:
	CMyObject() = default;
	CMyObject(const CMyObject&) = default;
	~CMyObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd);
	void	shutdown();
	bool	renderObject(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	int		getIndexCount();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	
	int m_vertexCount;
	int m_indexCount;

	bool	IsInit = false;
	
	CColorShaderClass* ObjectShader = nullptr;
};