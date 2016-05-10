#pragma once
#include "Singleton.h"

class CGameManager : public CSingleton<CGameManager>
{
public:
	CGameManager() = default;
	~CGameManager() = default;
	CGameManager(CGameManager&) = delete;

	void DestorySingleton() override
	{

	}

	void initialize();
	bool frame();

private:
};

