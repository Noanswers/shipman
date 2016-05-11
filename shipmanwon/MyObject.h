#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgi.h>

class CMyObject
{
public:
	CMyObject() = default;
	CMyObject(const CMyObject&) = default;
	~CMyObject() = default;

	bool	initialize(ID3D11Device*);
	void	shutdown();
	void	renderObject(ID3D11DeviceContext* deviceContext);
	int		getIndexCount();
	
	//////////////////////////////////////////////////////////////////////////
	//¸®¼Ò½º È¹µæÀ» À§ÇÑ º¯¼ö
	ID3D11ShaderResourceView*	m_TextureRV = nullptr;
	ID3D11SamplerState*			m_SamplerLinear = nullptr;

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
};

