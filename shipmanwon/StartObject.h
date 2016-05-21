#pragma once
#include "MyObject.h"
#include "config.h"
#include <vector>

class CStartObject :
	public CMyObject
{
public:
	CStartObject() = default;
	CStartObject(const CStartObject&) = default;
	virtual ~CStartObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;

	void	createObject();
};

