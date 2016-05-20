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
	//현재 위치
	auto p1Pos = player1->getCurrentPosition();
	auto p2Pos = player2->getCurrentPosition();

	//충돌 포인트
	auto colPoint = DirectX::XMFLOAT3((p1Pos.x + p2Pos.x) / 2, 0.0, (p1Pos.z + p2Pos.z) / 2);
	
	//충돌 벡터
	auto colVec = DirectX::XMFLOAT3(colPoint.x - p1Pos.x, 0.0f, colPoint.z = p1Pos.z);

	//진행 방향 벡터
	auto p1FV = player1->getForwardVector();
	auto p2FV = player2->getForwardVector();

	//질량
	float p1m = player1->getMass();
	float p2m = player2->getMass();

	//반발계수
	float e = player1->getCOR() <= player2->getCOR() ? player1->getCOR() : player2->getCOR();

	//현재 속력.
	float p1Speed = player1->getCurrentSpeed() + player1->getOuterSpeed();
	float p2Speed = player2->getCurrentSpeed() + player2->getOuterSpeed();

	//theta
	float theta = atan2((p1Pos.z - p2Pos.z), (p1Pos.x - p2Pos.x));
	//OutputDebugStringW(std::to_wstring(theta).c_str());

	//player1 속도 벡터 분해
	float p1xSpeed = p1Speed*cosf(player1->getForwardTheta().y);
	float p1zSpeed = -p1Speed*sinf(player1->getForwardTheta().y);

	//player2 속도 벡터 분해
	float p2xSpeed = p2Speed*cosf(player2->getForwardTheta().y);
	float p2zSpeed = -p2Speed*sinf(player2->getForwardTheta().y);

	//////////////////////////////////////////////////////////////////////////
	//충돌 판정에 대하여 케이스 분리

	float newP1x, newP1z, newP2x, newP2z;

	auto FV_Dot_product = p1FV.x * p2FV.x + p1FV.y * p2FV.y + p1FV.z * p2FV.z;

	//player new speed
	if ( FV_Dot_product > 0 )
	{
		OutputDebugStringW(L"scintil \n");
		//scintil's logic
		newP1x = p2xSpeed*(-colVec.x);
		newP1z = p2zSpeed*(-colVec.z);

		newP2x = p1xSpeed*(colVec.x);
		newP2z = p1zSpeed*(colVec.z);

		player1->setOuterSpeed(sqrt(newP1x*newP1x + newP1z*newP1z) * 3);
		player1->setCurrentSpeed(0);

		player2->setOuterSpeed(sqrt(newP2x*newP2x + newP2z*newP2z) * 3);
		player2->setCurrentSpeed(0);

		player1->setOuterTheta({ 0.0f, atan2(newP1z, newP1x), 0.0f });
		player2->setOuterTheta({ 0.0f, atan2(newP2z, newP2x), 0.0f });

		float theta1 = player1->getMoveTheta().y;
		float theta2 = player2->getMoveTheta().y;

		player1->setOuterVector(1.0f*cosf(-theta1), 0.0f, 1.0f*sinf(-theta1));
		player2->setOuterVector(1.0f*cosf(-theta2), 0.0f, 1.0f*sinf(-theta2));
	}
	else
	{
		OutputDebugStringW(L"jsfumato \n");
		//jsfumato's logic
		newP1x = (p1m - p2m*e) / (p1m + p2m)*p1xSpeed + (p2m + p2m*e) / (p1m + p2m)*p2xSpeed;
		newP1z = (p1m - p2m*e) / (p1m + p2m)*p1zSpeed + (p2m + p2m*e) / (p1m + p2m)*p2zSpeed;

		newP2x = (p2m - p1m*e) / (p1m + p2m)*p2xSpeed + (p1m + p1m*e) / (p1m + p2m)*p1xSpeed;
		newP2z = (p2m - p1m*e) / (p1m + p2m)*p2zSpeed + (p1m + p1m*e) / (p1m + p2m)*p1zSpeed;

		player1->setOuterSpeed(sqrt(newP1x*newP1x + newP1z*newP1z));
		player1->setCurrentSpeed(0);

		player2->setOuterSpeed(sqrt(newP2x*newP2x + newP2z*newP2z));
		player2->setCurrentSpeed(0);

		player1->setOuterTheta({ 0.0f, atan2(newP1z, newP1x), 0.0f });
		player2->setOuterTheta({ 0.0f, atan2(newP2z, newP2x), 0.0f });

		float theta1 = player1->getMoveTheta().y;
		float theta2 = player2->getMoveTheta().y;

		player1->setOuterVector(1.0f*cosf(-theta1), 0.0f, 1.0f*sinf(-theta1));
		player2->setOuterVector(1.0f*cosf(-theta2), 0.0f, 1.0f*sinf(-theta2));
	}
//
//	auto player1_speed = player1->getNowSpeed();
//	auto player2_speed = player2->getNowSpeed();
//
//	auto point1 = player1->getCurrentPosition(); //player1 current position
//	auto point2 = player2->getCurrentPosition(); //player2 current position
//
//	//collision point x
//	//auto cPointX = (pow(point1.x, 2) - pow(point2.x, 2) + pow(point1.y, 2) - pow(point2.y, 2)) / ( 2/(point1.x - point2.x) - 2*(point2.x - point1.x) ); 
//	auto cPointX = (point1.x + point2.x) / 2;
//
//	//collision point y
//	//auto cpointY = (point2.y - point1.y) / (point2.x - point1.x)*cPointX + point1.y + point2.y - (point2.y - point1.y)*(point1.x + point2.x) / (point2.x - point1.x);
//	auto cPointZ = (point1.z + point2.z) / 2;
//
//	if (-0.01 < cPointX && cPointX < 0.01)
//	{
//		cPointX = 0;
//	}
//
//	if (-1.0f < cPointZ && cPointZ < 1.0f)
//	{
//		cPointZ = 0;
//	}
//
//	//Collision Vector
//	DirectX::XMFLOAT3 cv1 = { cPointX - point1.x, point1.y, cPointZ - point1.z };
//	DirectX::XMFLOAT3 cv2 = { cPointX - point2.x, point2.y, cPointZ - point2.z };
//
//	//Forward Vector
//	auto fv1 = player1->getForwardVector();
//	auto fv2 = player2->getForwardVector();
//
//	//Dot Product
//	auto dot1 = (fv1.x * cv1.x) + (fv1.z * cv1.z);
//	auto dot2 = (fv2.x * cv2.x) + (fv2.z * cv2.z);
//
//	if (dot1 > dot2)
//	{
//		OutputDebugStringW(L"dot1 > dot2\n");
//		player1->moveBackward();
//	}
//
//	else if (dot1 == dot2)
//	{
//		OutputDebugStringW(L"dot1 == dot2\n");
//// 		player2->moveToward(-cv1.x, cv1.y, -cv1.z);
//// 		player1->moveToward(-cv2.x, cv2.y, -cv2.z);
//	}
//
//	else if (dot1 < dot2)
//	{
//		OutputDebugStringW(L"dot1 < dot2\n");
//		player2->moveBackward();
//	}
}
