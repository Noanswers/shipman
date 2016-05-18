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
				doCollision(playerVector[idx], playerVector[idx+1]);
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
