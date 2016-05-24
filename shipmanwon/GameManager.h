#pragma once
//#include "Singleton.h"
#include <vector>
#include "PlayerObject.h"
#include "StageObject.h"

class CGameManager
{
public:
	CGameManager() = default;
	~CGameManager() = default;
	CGameManager(CGameManager&) = delete;

	void initialize();
	bool frame(std::vector<CPlayerObject*> playerVector);

	bool isGameEnd(std::vector<CPlayerObject*> playerVector);
	void setStage(CStageObject* stage);

	CPlayerObject* getWinnerPlayer(std::vector<CPlayerObject*> playerVector);

private:
	//충돌
	void collisionCheck(std::vector<CPlayerObject*> playerVector);
	void doCollision(CPlayerObject* player1, CPlayerObject* player2);

	//추락
	void getOutCheck(std::vector<CPlayerObject*> playerVector);
	void doGetOut(CPlayerObject* player);
	
	CStageObject* currentStageObject = nullptr;
};