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

	bool isEnd(std::vector<CPlayerObject*> playerVector);
	void doEnd();
	void setStage(CStageObject* stage);

private:
	//�浹
	void collisionCheck(std::vector<CPlayerObject*> playerVector);
	void doCollision(CPlayerObject* player1, CPlayerObject* player2);

	//�߶�
	void getOutCheck(std::vector<CPlayerObject*> playerVector);
	void doGetOut(CPlayerObject* player);

	//��� üũ
	void resultCheck(std::vector<CPlayerObject*> playerVector);
	
	bool test_checker = false;
	bool getout_checker = false;
	
	CPlayerObject* winPlayer = nullptr;
	CStageObject* currentStageObject = nullptr;

	int numPlayer = 2; //setting �ʿ�
};