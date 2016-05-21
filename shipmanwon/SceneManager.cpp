#include "stdafx.h"
#include "SceneManager.h"
#include "StartScene.h"
#include "GameScene.h"

void CSceneManager::initialize()
{
	SceneStack.clear();

	CStartScene* startScene = new CStartScene();

	CGameScene* gameScene = new CGameScene();
	
	SceneStack.push_back(gameScene);
	SceneStack.push_back(startScene);
}

bool CSceneManager::pushBack(CMyScene* scene)
{
	size_t preSize = SceneStack.size();
	SceneStack.push_back(scene);

	size_t currentSize = SceneStack.size();
	if (currentSize == (preSize + 1))
		return true;
	return false;
}

bool CSceneManager::popBack()
{
	size_t preSize = SceneStack.size();
	SceneStack.pop_back();

	size_t currentSize = SceneStack.size();
	if (currentSize == (preSize - 1))
		return true;
	return false;
}

CMyScene* CSceneManager::getCurrentScene()
{
	if (SceneStack.empty())
		return nullptr;

	return SceneStack.back();
}

int CSceneManager::getStackSize()
{
	return static_cast<int>(SceneStack.size());
}

void CSceneManager::update()
{
	CMyScene* currentScene = getCurrentScene();
	

}