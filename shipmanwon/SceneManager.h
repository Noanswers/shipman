#pragma once

#include <vector>
#include "Singleton.h"
#include "MyObject.h"
#include "MyLayer.h"
#include "MyScene.h"

class CSceneManager : public CSingleton<CSceneManager>
{
public:
	CSceneManager() = default;
	~CSceneManager() = default;
	CSceneManager(CSceneManager&) = delete;

	void initialize();
	bool pushBack(CMyScene* scene);
	bool popBack();
	CMyScene* getCurrentScene();

	int getStackSize();
	void update();

	

private:
	std::vector<CMyScene*> SceneStack;
};