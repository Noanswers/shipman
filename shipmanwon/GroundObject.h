#pragma once
#include "MyObject.h"

class CGroundObject : public CMyObject
{
public:
	CGroundObject() = default;
	CGroundObject(const CGroundObject&) = default;
	~CGroundObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	void	createShader() override;

private:
	void	createGround();

};

