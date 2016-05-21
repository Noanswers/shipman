#pragma once
#include "MyObject.h"

class CTreeObject : public CMyObject
{
public:
	CTreeObject() = default;
	CTreeObject(const CTreeObject&) = default;
	~CTreeObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	void	createShader() override;

private:
	void	createTree();

};

