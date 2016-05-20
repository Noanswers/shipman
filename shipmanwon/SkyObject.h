#pragma once
#include "MyObject.h"

class CSkyObject : public CMyObject
{
public:
	CSkyObject() = default;
	CSkyObject(const CSkyObject&) = default;
	~CSkyObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;

	//bool renderObject(ID3D11DeviceContext * deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc);

	//bool renderObject(ID3D11DeviceContext * deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc, ID3D11Buffer * SkyConstantBuffer);

	//bool renderObject(ID3D11DeviceContext * deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc) override;

	void createShader() override;

private:
	/*struct SkyVertexType
	{
		DirectX::XMFLOAT3 position;
	};

	struct skyConstantBuffer
	{
		XMMATRIX wvp;
	};*/

	//std::vector<SkyVertexType> SkyVertices;

	void createSkyVertexShader(ID3DBlob* pVSBlob, ID3DBlob* pErrorBlob);
	void createSkyPixelShader(ID3DBlob * pPSBlob, ID3DBlob * pErrorBlob);
	void createSkyVertexBufferLayout(ID3DBlob * pVSBlob);

	void createSkyVertexBuffer();

	ID3D11VertexShader*	SkyVertexShader = nullptr;
	ID3D11PixelShader*	SkyPixelShader = nullptr;

	ID3D11Buffer*	SkyVertBuffer = nullptr;
	ID3D11Buffer*   SkyIndexBuffer = nullptr;

	ID3D11InputLayout*	SkyVertexLayout = nullptr;
	ID3D11RasterizerState *      SolidRS = nullptr;
	ID3D11DepthStencilState * DepthStencilLessEqual = nullptr;

	void loadSkyTexture();
	void createRenderState();
	void createDepthStencil();

	void createSphere(float radius, int sliceCount, int stackCount);

	void renderBuffers(ID3D11DeviceContext * deviceContext);

	//void renderBuffers(ID3D11DeviceContext * deviceContext, ID3D11Buffer * SkyConstantBuffer);

	//void renderBuffers(ID3D11DeviceContext * deviceContext) override;

	//void createSphere(float radius, UINT numSubdivisions);
	//void createSkyVertexBufferLayout();
	bool initializeBuffers(ID3D11Device * device) override;
	void createSkyIndexBuffer();
};

