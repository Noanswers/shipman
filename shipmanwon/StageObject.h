#pragma once
#include "MyObject.h"

class CStageObject : public CMyObject
{
public:
	CStageObject() = default;
	~CStageObject() = default;
	
	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	bool	renderObject(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) override;

private:

	void	renderBuffers(ID3D11DeviceContext* deviceContext) override;
	VertexType vertices[8] =
	{
		{ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.0f, -4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-6.0f, 0.0f, -2.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.0f, 4.0f, -2.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.0f, -4.0f, -2.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(6.0f, 0.0f, -2.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
	};

	unsigned long indices[36] =
	{
		0, 1, 2, 2, 1, 3,
		4, 0, 2, 4, 2, 6, 6, 2, 3, 6, 3, 7,
		7, 3, 1, 7, 1, 5, 5, 1, 0, 5, 0, 4,
		4, 6, 5, 6, 7, 4
	};

	int m_vertexCount = 8;
	int m_indexCount = 36;
};

