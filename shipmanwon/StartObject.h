#pragma once
#include "MyObject.h"
#include <vector>

class CStartObject :
	public CMyObject
{
public:
	CStartObject();
	~CStartObject();


	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	bool	renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc) override;
	void	createTitleBox(void);

	

private:

	//void createTitleBox();
	std::wstring textureDefault = L"";

	ID3D11Resource* Resource;

	ID3D11Device*				temp_device = nullptr;
	ID3D11ShaderResourceView*	m_pTextureRV = nullptr;
	ID3D11SamplerState*			m_pSamplerLinear = nullptr;

	HRESULT loadTexture(); //override???
	void createShader();

	void	renderBuffers(ID3D11DeviceContext* deviceContext) override;
	bool	initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices, int m_vertexCount, int m_indexCount);

	

	std::vector<VertexType> stVerticies;
	std::vector<unsigned long> stIndices;



};

