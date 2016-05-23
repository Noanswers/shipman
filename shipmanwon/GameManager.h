#pragma once
#include "Singleton.h"
#include <vector>
#include "PlayerObject.h"
#include "StageObject.h"

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

	//충돌
	void collisionCheck(std::vector<CPlayerObject*> playerVector);
	void doCollision(CPlayerObject* player1, CPlayerObject* player2);

	//추락
	void getOutCheck(std::vector<CPlayerObject*> playerVector, CStageObject* stage);
	void doGetOut(CPlayerObject* player);

	void resultCheck(std::vector<CPlayerObject*> playerVector);

	bool isEnd(std::vector<CPlayerObject*> playerVector);
	void doEnd();

private:
	bool test_checker = false;
	bool getout_checker = false;
	CPlayerObject* winPlayer;
	int numPlayer = 2; //setting 필요
};