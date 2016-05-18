#include "stdafx.h"
#include "GameManager.h"
#include "SceneManager.h"

void CGameManager::initialize()
{

}

bool CGameManager::frame()
{
	//CMyScene* curentScene = CSceneManager::GetInstance()->getCurrentScene();	
	return true;
}

void CGameManager::update()
{
	CMyScene* currentScene = CSceneManager::GetInstance()->getCurrentScene();
}