#include "stdafx.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ResultScene.h"
#include "ResultObject.h"

void CGameManager::initialize()
{

}

bool CGameManager::frame()
{
	//update();
	return true;
}

void CGameManager::collisionCheck(std::vector<CPlayerObject*> playerVector)
{
	int numOfPlayer = 2;
	int idx = 0;

	for (auto& iter : playerVector)
	{
		if (test_checker)
		{
			test_checker = false;
		}

		if ((idx + 1) < numOfPlayer)
			if (iter->isCollisionPlayer(playerVector[idx + 1]))
			{
				doCollision(playerVector[idx], playerVector[idx + 1]);
				test_checker = true;
			}


		else
			break;

		idx++;
	}

}

void CGameManager::doCollision(CPlayerObject* player1, CPlayerObject* player2)
{
	player1->moveBackward();
	player2->moveBackward();
}

void CGameManager::getOutCheck(std::vector<CPlayerObject*> playerVector, CStageObject* stage)
{

	for (auto& iter : playerVector)
	{
		DirectX::XMFLOAT3 currentPos = iter->getCurrentPosition();
		if (stage->isGetOutStage(currentPos))
		{
			doGetOut(iter);
		}
	}
}
void CGameManager::doGetOut(CPlayerObject* player)
{
	player->SetOutPlayer(true);

	OutputDebugStringA("out\n");

	
}

void CGameManager::resultCheck(std::vector<CPlayerObject*> playerVector)
{

	if (isEnd(playerVector))
	{
		doEnd();
	}
	
}

bool CGameManager::isEnd(std::vector<CPlayerObject*> playerVector)
{
	int playerCount = 0;
	
	CPlayerObject* winPlayer;
	for (auto& i : playerVector)
	{
		if (i->GetOutPlayer())
			playerCount++;
		else
			winPlayer = i;
	}

	if (playerCount == (numPlayer - 1))
	{
		this->winPlayer = winPlayer;
		return true;
	}

	return false;

}

void CGameManager::doEnd()
{
	CResultScene* resultScene = new CResultScene();

	resultScene->initialize();
	CSceneManager::GetInstance()->pushBack(resultScene);

	CResultObject* resultObject = new CResultObject();
	resultObject->SetWinPlayerNum(winPlayer->GetplayerNumber());
	resultScene->pushBack(resultObject, 10);
}