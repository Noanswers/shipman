#pragma once
#include "MyObject.h"
class CResultObject :
	public CMyObject
{
public:
	CResultObject() = default;
	CResultObject(const CResultObject&) = default;
	virtual ~CResultObject() = default;


	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;

	void createObject();

	


	void SetWinPlayerNum(int set)
	{
		winPlayerNum = set;
	}
private:
	int winPlayerNum;
};

