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
	bool	renderObject(
		ID3D11DeviceContext* deviceContext, 
		std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc
		) override;

	//void	CStageObject::renderBuffers(ID3D11DeviceContext* deviceContext) override;
	void	createCylinder(void);
	void	createTopCap(void);
	void	createBottomCap(void);
	void	update() override;

private:
	std::wstring textureFilename = std::wstring();
	ID3D11Resource* Resource;

	ID3D11Device*				temp_device = nullptr;
	ID3D11ShaderResourceView*	m_pTextureRV = nullptr;
	ID3D11SamplerState*			m_pSamplerLinear = nullptr;

	HRESULT loadTexture(); //override???
	void createShader();

	void	renderBuffers(ID3D11DeviceContext* deviceContext) override;
	bool	initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices, int m_vertexCount, int m_indexCount);


	//////////////////////////////////////////////////////////////////////////
	//원기둥 그리는 변수들
	float bottomRadius = 0.8f;
	float topRadius = 0.8f;
	float height = 0.8f;
	UINT sliceCount = 100;
	UINT stackCount = 5;
	

	////////////////플레이어 정보
	float speed = 0.01f;


	std::vector<VertexType> cyVerticies;
	std::vector<unsigned long> cyIndices;
};

