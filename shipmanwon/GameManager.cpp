#include "stdafx.h"
#include "GameManager.h"
#include "SceneManager.h"

void CGameManager::initialize()
{

}

bool CGameManager::frame()
{
	//update();
	return true;
}

void CGameManager::update()
{
	CMyScene* currentScene = CSceneManager::GetInstance()->getCurrentScene();
	currentScene->update();
}

void CGameManager::collisionCheck(std::vector<CPlayerObject*> playerVector)
{
	int numOfPlayer = 2;
	int idx = 0;
	
	for (auto& iter : playerVector)
	{
		if ((idx + 1) < numOfPlayer)
			if (iter->isCollisionPlayer(playerVector[idx+1]))
				doCollision(iter);
		else
			break;
		idx++;
	}

}


void CGameManager::doCollision(CPlayerObject* player)
{
	player->setTranslate(-1, 0, -1);
}
