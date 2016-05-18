#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <DirectXMath.h>
#include <WICTextureLoader.h>
#include <string>
#include <functional>

class CMyObject
{
public:
	CMyObject() = default;
	CMyObject(const CMyObject&) = default;
	virtual ~CMyObject() = default;

	virtual bool	initialize(ID3D11Device* device, HWND hWnd);
	virtual void	shutdown();
	virtual bool	renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc);

	int		getIndexCount();

	void	moveToward(float x, float y, float z);
	void	moveToward(float x, float y, float z, float now_speed);
	void	moveForward();
	void	moveBackward();
	void	boost();
	void	setboostSpeed();
	void	resetSpeed();
	float	getNowSpeed();
	DirectX::XMFLOAT3	getForwardVector();

	void	setRotate(float x, float y, float z);
	void	setScale(float x, float y, float z);
	void	setTranslate(float x, float y, float z);
	void	setColorRGBA(float red, float green, float blue, float alpha);

	DirectX::XMMATRIX getWorldMatrix();

	DirectX::XMFLOAT3 getCurrentPosition ()
	{return currentPosition;}

	void	setCurrentPosition(float x, float y, float z);

	float CalcDistanceTwoPoint(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

protected:

	DirectX::XMFLOAT3 currentPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 ForwardTheta = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 ForwardVector = { 1.0f, 0.0f, 0.0f };

	float speed = 0.05f;

	std::wstring textureFilename = std::wstring();
	ID3D11Resource* Resource;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;

		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
	};

	ID3D11Device*				temp_device = nullptr;
	ID3D11ShaderResourceView*	TextureRV = nullptr;
	ID3D11SamplerState*			SamplerLinear = nullptr;

	HRESULT loadTexture();
	void createShader();

	void renderBuffers(ID3D11DeviceContext*);
//	virtual bool initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices, int m_vertexCount, int m_indexCount);
	virtual bool initializeBuffers(ID3D11Device* device);

	virtual void shutdownBuffers();

	/*int m_vertexCount;
	int m_indexCount;
*/
	std::vector<VertexType> Verticies;
	std::vector<unsigned long> Indices;

	/*VertexType vertices[4] = 
	{
		{ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.0f, -4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }
	};

	unsigned long indices[6] =
	{
		0, 1, 2, 2, 1, 3
	};*/

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11InputLayout*	m_pVertexLayout = NULL;
	ID3D11PixelShader*	m_pPixelShader = NULL;

	bool	IsInit = false;
	
	DirectX::XMMATRIX ObjectScale = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ObjectRotate = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ObjectTranslate = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX ObjectWorld = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ObjectView = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ObjectProjection = DirectX::XMMatrixIdentity();
};