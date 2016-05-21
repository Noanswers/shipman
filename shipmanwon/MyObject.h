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

	bool	renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc);
	int		getIndexCount();

	void	moveToward(float x, float y, float z);
	void	moveTowardByOuter(float x, float y, float z);
	void	moveForward();
	void	moveBackward();
	void	moveStop();

	void	accelerate();
	
	void	boost();
	void	setMaximumSpeed(float speed);

	void	resetSpeed();
	float	getCurrentSpeed() const;

	DirectX::XMFLOAT3	getForwardVector();
	DirectX::XMFLOAT3	getForwardTheta() const;

	void	setRotate(float x, float y, float z);
	void	setScale(float x, float y, float z);
	void	setTranslate(float x, float y, float z);
	void	setColorRGBA(float red, float green, float blue, float alpha);
	void	setForwardVector(float x, float y, float z);

	DirectX::XMMATRIX getWorldMatrix();

	DirectX::XMFLOAT3 getCurrentPosition ()
	{return currentPosition;}

	void	setCurrentPosition(float x, float y, float z);
	std::string getObjectName() const;
	void	setObjectName(std::string objName);
	float	getMass();
	float	CalcDistanceTwoPoint(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
	void	setTexture(std::wstring texName);
	float	getCOR() const;
	void	setCOR(float cor);

	void	setCurrentSpeed(float speed);
	void	setForwardTheta(DirectX::XMFLOAT3 theta);
	void	setOuterTheta(DirectX::XMFLOAT3 theta);
	void	setOuterVector(float x, float y, float z);
	DirectX::XMFLOAT3 getMoveTheta();
	float	getOuterSpeed() const;
	void	setOuterSpeed(float speed);
	
	void	move();

protected:
	//void shutdownBuffers();
	float ObjectCOR = 1.9;
	float ObjectMass = 2.0f;

	DirectX::XMFLOAT3 currentPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 ForwardTheta = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 ForwardVector = { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 OuterTheta = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 OuterVector = { 1.0f, 0.0f, 0.0f };

	float SpeedDelta = 0.0f;
	float CurrentSpeed = 0.0f;
	float OuterSpeed = 0.0f;
	float MaximumSpeed = 0.08f;

	std::wstring textureFilename = std::wstring();
	std::string	 ObjectName = std::string();
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
	virtual bool initializeBuffers(ID3D11Device* device);

	virtual void shutdownBuffers();

	std::vector<VertexType> Verticies;
	std::vector<unsigned long> Indices;

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
