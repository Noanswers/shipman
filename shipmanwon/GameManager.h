#pragma once
#include "Singleton.h"
#include "SceneManager.h"
#include "MyScene.h"

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
	void update();

private:
};