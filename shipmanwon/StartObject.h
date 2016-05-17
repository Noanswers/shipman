#pragma once
#include "MyObject.h"
class CStartObject :
	public CMyObject
{
public:
	CStartObject();
	~CStartObject();


	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	bool	renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc) override;
	
	

private:

	void createTitleBox();

	const std::wstring textureDefault = L"";
};

