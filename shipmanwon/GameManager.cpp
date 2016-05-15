#include "stdafx.h"
#include "GameManager.h"

void CGameManager::initialize()
{

}

bool CGameManager::frame()
{
	CMyScene* curentScene = CSceneManager::GetInstance()->getCurrentScene();

	
	curentScene->update();

	return true;
}