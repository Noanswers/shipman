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
	//CMyScene* curentScene = CSceneManager::GetInstance()->getCurrentScene();	
	return true;
}

void CGameManager::collisionCheck(std::vector<CPlayerObject*> playerVector)
{
	if (playerVector.size() < 2)
		return;

	for (auto iter = playerVector.begin(); iter < playerVector.end() - 1; ++iter)
	{
		for (auto iter2 = (iter + 1); iter2 < playerVector.end(); ++iter2)
		{
			if((*iter)->isCollisionPlayer(*iter2))
				doCollision(*iter, *iter2);
		}
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
	auto colVec = DirectX::XMFLOAT3(colPoint.x - p1Pos.x, 0.0f, colPoint.z - p1Pos.z);

	//진행 방향 벡터
	auto p1FV = player1->getForwardVector();
	auto p2FV = player2->getForwardVector();

	//질량
	auto p1m = player1->getMass();
	auto p2m = player2->getMass();

	//반발계수
	auto e = player1->getCOR() <= player2->getCOR() ? player1->getCOR() : player2->getCOR();

	//현재 속력.
	auto p1Speed = player1->getCurrentSpeed() + player1->getOuterSpeed();
	auto p2Speed = player2->getCurrentSpeed() + player2->getOuterSpeed();

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

	//Forward Vector에 대하여 내적을 진행하여 각도에 따라 충돌을 분리
	auto FV_Dot_product = p1FV.x * p2FV.x + p1FV.y * p2FV.y + p1FV.z * p2FV.z;

	//물체가 비벼질 경우 물체의 위치가 겹쳐서 충돌이 계속 일어나는 경우가 발생.
	//최대한 방지 하기 위해 충돌시 물체의 현재 위치를 충돌 포인트에서 조금씩 뒤로 띄워줌.
	player1->moveToward(-colVec.x, colVec.y , -colVec.z);
	player2->moveToward(colVec.x, colVec.y, colVec.z);

	//player new speed
	if ( FV_Dot_product > 0 )
	{
		//////////////////////////////////////////////////////////////////////////
		//Logic.1
		//OutputDebugStringW(L"scintil \n");

		newP1x = p2xSpeed*(-colVec.x);
 		newP1z = p2zSpeed*(-colVec.z);
 
 		newP2x = p1xSpeed*(colVec.x);
 		newP2z = p1zSpeed*(colVec.z);

		float newP1OuterSpeed = sqrt(newP1x*newP1x + newP1z*newP1z);
		float newP2OuterSpeed = sqrt(newP2x*newP2x + newP2z*newP2z);

		player1->setOuterSpeed(newP1OuterSpeed * 3);
		player1->setCurrentSpeed(0);

		player2->setOuterSpeed(newP2OuterSpeed * 3);
		player2->setCurrentSpeed(0);

		player1->setOuterTheta({ 0.0f, DirectX::XM_2PI- atan2(newP1z, newP1x), 0.0f });
		player2->setOuterTheta({ 0.0f, DirectX::XM_2PI- atan2(newP2z, newP2x), 0.0f });
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		//Logic.2
		//OutputDebugStringW(L"jsfumato \n"); 

		newP1x = (p1m - p2m*e) / (p1m + p2m)*p1xSpeed + (p2m + p2m*e) / (p1m + p2m)*p2xSpeed;
		newP1z = (p1m - p2m*e) / (p1m + p2m)*p1zSpeed + (p2m + p2m*e) / (p1m + p2m)*p2zSpeed;

		newP2x = (p2m - p1m*e) / (p1m + p2m)*p2xSpeed + (p1m + p1m*e) / (p1m + p2m)*p1xSpeed;
		newP2z = (p2m - p1m*e) / (p1m + p2m)*p2zSpeed + (p1m + p1m*e) / (p1m + p2m)*p1zSpeed;

		auto euler = (float)exp(1);

		float newP1OuterSpeed = sqrt(newP1x*newP1x + newP1z*newP1z);
		float newP2OuterSpeed = sqrt(newP2x*newP2x + newP2z*newP2z);

		//P1 벡터 합성
		//auto newP1OuterSpeed = sqrt( log( newP1x*newP1x + newP1z*newP1z + euler) ) - 0.8f;

		player1->setOuterSpeed( newP1OuterSpeed );
		player1->setCurrentSpeed(0);

		//P2 벡터 합성
		//auto newP2OuterSpeed = sqrt( log( newP2x*newP2x + newP2z*newP2z + euler) ) - 0.8f;

		player2->setOuterSpeed( newP2OuterSpeed );
		player2->setCurrentSpeed(0);

		player1->setOuterTheta({ 0.0f, DirectX::XM_2PI - atan2(newP1z, newP1x), 0.0f });
		player2->setOuterTheta({ 0.0f, DirectX::XM_2PI - atan2(newP2z, newP2x), 0.0f });
	}

	//충돌후 벡터 방향
	float theta1 = player1->getMoveTheta().y;
	float theta2 = player2->getMoveTheta().y;

	player1->setOuterVector(1.0f*cosf(-theta1), 0.0f, 1.0f*sinf(-theta1));
	player2->setOuterVector(1.0f*cosf(-theta2), 0.0f, 1.0f*sinf(-theta2));
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
	//if (player->getCurrentPosition().y > -10000.0f)
	player->dropDown(0.1f);

	if(player->getCurrentPosition().y <= -20)
		player->SetOutPlayer(true);


	
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