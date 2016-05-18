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
	auto player1_speed = player1->getNowSpeed();
	auto player2_speed = player2->getNowSpeed();

	auto point1 = player1->getCurrentPosition(); //player1 current position
	auto point2 = player2->getCurrentPosition(); //player2 current position

	//collision point x
	//auto cPointX = (pow(point1.x, 2) - pow(point2.x, 2) + pow(point1.y, 2) - pow(point2.y, 2)) / ( 2/(point1.x - point2.x) - 2*(point2.x - point1.x) ); 
	auto cPointX = (point1.x + point2.x) / 2;

	//collision point y
	//auto cpointY = (point2.y - point1.y) / (point2.x - point1.x)*cPointX + point1.y + point2.y - (point2.y - point1.y)*(point1.x + point2.x) / (point2.x - point1.x);
	auto cPointZ = (point1.z + point2.z) / 2;

	if (-0.01 < cPointX < 0.01)
	{
		cPointX = 0;
	}

	if (-1.0f < cPointZ || cPointZ < 1.0f)
	{
		cPointZ = 0;
	}

	//Collision Vector
	DirectX::XMFLOAT3 cv1 = { cPointX - point1.x, point1.y, cPointZ - point1.z };
	DirectX::XMFLOAT3 cv2 = { cPointX - point2.x, point2.y, cPointZ - point2.z };

	//Forward Vector
	auto fv1 = player1->getForwardVector();
	auto fv2 = player2->getForwardVector();

	//Dot Product
	auto dot1 = (fv1.x * cv1.x) + (fv1.z * cv1.z);
	auto dot2 = (fv2.x * cv2.x) + (fv2.z * cv2.z);

	if (dot1 > dot2)
	{
		OutputDebugStringW(L"dot1 > dot2\n");
		player1->moveBackward();
	}

	else if (dot1 == dot2)
	{
		OutputDebugStringW(L"dot1 == dot2\n");
// 		player2->moveToward(-cv1.x, cv1.y, -cv1.z);
// 		player1->moveToward(-cv2.x, cv2.y, -cv2.z);
	}

	else if (dot1 < dot2)
	{
		OutputDebugStringW(L"dot1 < dot2\n");
		player2->moveBackward();
	}
}
