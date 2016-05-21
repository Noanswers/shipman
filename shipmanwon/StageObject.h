#pragma once
#include "MyObject.h"


class CStageObject : public CMyObject
{
public:
	CStageObject() = default;
	CStageObject(const CStageObject&) = default;
	~CStageObject() = default;
	
	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;

	bool	isGetOutStage(DirectX::XMFLOAT3 position);

private:
	void createStage();
	//ÀÓ½Ã°ª
	DirectX::XMFLOAT3 realPosition[4];
	
	
		
		
	
};

