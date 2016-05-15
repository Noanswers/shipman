#pragma once
#include "MyObject.h"
#include <vector>

class CPlayerObject : public CMyObject
{
public:
	CPlayerObject() = default;
	CPlayerObject(const CPlayerObject&) = default;
	~CPlayerObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	bool	renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc) override;

	//void	CStageObject::renderBuffers(ID3D11DeviceContext* deviceContext) override;
	void	createCylinder(void);
	void	createTopCap(void);
	void	createBottomCap(void);

private:
	std::wstring textureFilename = std::wstring();
	ID3D11Resource* Resource;

	ID3D11Device*				temp_device = nullptr;
	ID3D11ShaderResourceView*	g_pTextureRV = nullptr;
	ID3D11SamplerState*			g_pSamplerLinear = nullptr;

	HRESULT loadTexture(); //override???
	void createShader();

	void	renderBuffers(ID3D11DeviceContext* deviceContext) override;
	bool	initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices, int m_vertexCount, int m_indexCount);

	int m_vertexCount = 4;
	int m_indexCount = 6;

	//////////////////////////////////////////////////////////////////////////
	//����� �׸��� ������
	float bottomRadius = 2.0f;
	float topRadius = 2.0f;
	float height = 0.2f;
	UINT sliceCount = 10;
	UINT stackCount = 5;

	std::vector<VertexType> cyVerticies;
	std::vector<unsigned long> cyIndices;
};

