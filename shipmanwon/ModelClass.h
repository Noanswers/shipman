#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class CModelClass
{


public:
	CModelClass()
	{
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;
	};
	CModelClass(const CModelClass&) = default;
	~CModelClass() = default;

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};
