#pragma once
#include "Singleton.h"
#include <vector>
#include "PlayerObject.h"

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
	void collisionCheck(std::vector<CPlayerObject*> playerVector);
	void doCollision(CPlayerObject* player);
private:
};