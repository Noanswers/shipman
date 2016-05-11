#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <array>
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
	void	setPosition(float x, float y, float z);

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	float xPos = 0;
	float yPos = 0;
	float zPos = 0;

	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

	int m_vertexCount = 4;
	int m_indexCount = 6;

	VertexType vertices[4] = 
	{
		{ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.0f, -4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	unsigned long indices[6] =
	{
		0, 1, 2, 2, 1, 3
	};

	//VertexType* m_vertices = nullptr;

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	bool	IsInit = false;
	
	CColorShaderClass* ObjectShader = nullptr;
};