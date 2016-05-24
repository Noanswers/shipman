#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <DirectXMath.h>
#include <WICTextureLoader.h>
#include <string>
#include <functional>

using namespace DirectX;

class CMyObject
{
public:
	CMyObject() = default;
	CMyObject(const CMyObject&) = default;
	virtual ~CMyObject() 
	{
		Verticies.clear();
		Indices.clear();

		if (Resource)		{ Resource->Release();			}
		if (pTextureRV)		{ pTextureRV->Release();		}
		if (pSamplerLinear) { pSamplerLinear->Release();	}
		if (pVertexBuffer)	{ pVertexBuffer->Release();		}
		if (pIndexBuffer)	{ pIndexBuffer->Release();		}
		if (pVertexShader)	{ pVertexShader->Release();		}
		if (pVertexLayout)	{ pVertexLayout->Release();		}
		if (pPixelShader)	{ pPixelShader->Release();		}
	}; 

	virtual bool	initialize(ID3D11Device* device, HWND hWnd);
	virtual void	shutdown();

	virtual bool	renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc);
	int				getIndexCount();
	float			calcDistanceTwoPoint(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b); //for Collision Check

	//Object Moving
	void			move();
	void			moveToward(float x, float y, float z);
	void			moveTowardByOuter(float x, float y, float z);
	void			moveForward();
	void			moveBackward();
	void			moveStop();
	void			accelerate();
	void			boost();
	void			resetSpeed();

	//Set
	void			setCurrentPosition(float x, float y, float z);
	void			setRotate(float x, float y, float z);
	void			setScale(float x, float y, float z);
	void			setTranslate(float x, float y, float z);
	void			setColorRGBA(float red, float green, float blue, float alpha);
	void			setForwardVector(float x, float y, float z);
	void			setObjectName(std::string objName);
	void			setTexture(std::wstring texName);
	void			setCOR(float cor);
	void			setCurrentSpeed(float speed);
	void			setMaximumSpeed(float speed);
	void			setForwardTheta(DirectX::XMFLOAT3 theta);
	void			setOuterTheta(DirectX::XMFLOAT3 theta);
	void			setOuterVector(float x, float y, float z);
	void			setOuterSpeed(float speed);
	
	//Get
	std::string			getObjectName() const;
	float				getCurrentSpeed() const;
	float				getMass() const;
	float				getCOR() const;
	float				getOuterSpeed() const;
	DirectX::XMMATRIX	getWorldMatrix() const;
	DirectX::XMFLOAT3	getCurrentPosition() const { return currentPosition; }
	DirectX::XMFLOAT3	getMoveTheta() const;
	DirectX::XMFLOAT3	getForwardVector() const;
	DirectX::XMFLOAT3	getForwardTheta() const;

protected:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;

		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
	};

	bool	IsInit = false;

	float ObjectCOR		= 1.9;
	float ObjectMass	= 2.0f;
	float SpeedDelta	= 0.0f;
	float CurrentSpeed	= 0.0f;
	float OuterSpeed	= 0.0f;
	float MaximumSpeed	= 0.08f;

	DirectX::XMFLOAT3 currentPosition	= { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 ForwardTheta		= { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 ForwardVector		= { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 OuterTheta		= { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 OuterVector		= { 1.0f, 0.0f, 0.0f };

	std::string	 ObjectName = std::string();
	std::wstring textureFilename = std::wstring();
	ID3D11Resource* Resource;

	ID3D11Device*				pTemp_Device = nullptr;
	ID3D11ShaderResourceView*	pTextureRV = nullptr;
	ID3D11SamplerState*			pSamplerLinear = nullptr;

	//Shader
	std::vector<VertexType>		Verticies;
	std::vector<unsigned long>	Indices;
	ID3D11Buffer*				pVertexBuffer = nullptr;
	ID3D11Buffer*				pIndexBuffer = nullptr;
	ID3D11VertexShader*			pVertexShader = nullptr;
	ID3D11InputLayout*			pVertexLayout = nullptr;
	ID3D11PixelShader*			pPixelShader = nullptr;

	HRESULT				loadTexture();

	virtual void		createShader();
	virtual void		shutdownBuffers();
	virtual void		renderBuffers(ID3D11DeviceContext*);
	virtual bool		initializeBuffers(ID3D11Device* device);

	//Object Matrix
	DirectX::XMMATRIX ObjectScale		= DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ObjectRotate		= DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ObjectTranslate	= DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX ObjectWorld		= DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ObjectView		= DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ObjectProjection	= DirectX::XMMatrixIdentity();
};
