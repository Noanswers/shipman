#include "stdafx.h"
#include "SceneManager.h"

void CSceneManager::initialize()
{
	SceneStack.clear();
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

CMyScene* CSceneManager::getCurrentScene()
{
	if (SceneStack.empty())
		return nullptr;

	return SceneStack.back();
}